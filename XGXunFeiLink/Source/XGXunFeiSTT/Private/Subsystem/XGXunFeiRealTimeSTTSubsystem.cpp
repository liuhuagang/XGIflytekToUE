// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiRealTimeSTTSubsystem.h"

#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "JsonObjectConverter.h"
#include "HAL/RunnableThread.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"

#include "LogXGXunFeiSTT.h"
#include "XGXunFeiRealTimeSTTRespType.h"
#include "XGXunFeiAudioCaptureSubsystem.h"
#include "XGXunFeiConsumeVoiceRunnable.h"



bool UXGXunFeiRealTimeSTTSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGXunFeiRealTimeSTTSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	Super::Initialize(Collection);
}

void UXGXunFeiRealTimeSTTSubsystem::Deinitialize()
{
	XGStopRealTimeSpeechToText();

	Super::Deinitialize();

}

void UXGXunFeiRealTimeSTTSubsystem::XGBeginRealTimeSpeechToText(
	FXGXunFeiRealTimeSTTReqInfo& InRealTimeSTTReqInfo,
	FXGXunFeiInitRealTimeSTTDelegate InInitRealTimeSTTDelegate,
	FXGXunFeiRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate,
	FXGXunFeiRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate)
{
	if (ReakTimeSTTStatus != EXGXunFeiRealTimeSTTStatus::Ready)
	{
		InInitRealTimeSTTDelegate.ExecuteIfBound(false, TEXT("XGXunFeiRealTimeSTT is Running !"));

		UE_LOG(LogXGXunFeiSTT, Warning, TEXT("[%s] try to connect to iFLyTek,But It is Running"), *FString(__FUNCTION__));

		return;
	}

	UE_LOG(LogXGXunFeiSTT, Display, TEXT("[%s] is trying to connect to iFLyTek"), *FString(__FUNCTION__));

	ReakTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Init;

	InitRealTimeSTTDelegate = InInitRealTimeSTTDelegate;
	RealTimeSTTNoTranslateDelegate = InRealTimeSTTNoTranslateDelegate;
	RealTimeSTTTranslateDelegate = InRealTimeSTTTranslateDelegate;

	FModuleManager::Get().LoadModuleChecked("WebSockets");
	FString ServerProtocol = TEXT("ws");
	FString ServerURL = TEXT("ws://rtasr.xfyun.cn/v1/ws?");
	ServerURL += InRealTimeSTTReqInfo.GenerateRequireParams();


	AsyncTask(ENamedThreads::GameThread, [ServerURL, ServerProtocol, this]() {

		Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
		Socket->OnConnected().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnConnected);
		Socket->OnConnectionError().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnConnectionError);
		Socket->OnClosed().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnClosed);
		Socket->OnMessage().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnMessage);
		Socket->OnMessageSent().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnMessageSent);

		Socket->Connect();
		});




}

void UXGXunFeiRealTimeSTTSubsystem::XGStopRealTimeSpeechToText()
{
	if (ReakTimeSTTStatus == EXGXunFeiRealTimeSTTStatus::Ready)
	{
		//UE_LOG(LogXGXunFeiSTT, Warning, TEXT("[%s] try to close iFLyTek connect,but It was closed"), *FString(__FUNCTION__));
		return;
	}

	//UE_LOG(LogXGXunFeiSTT, Warning, TEXT("[%s] try to close iFLyTek Connect"), *FString(__FUNCTION__));
	InitRealTimeSTTDelegate.Clear();
	RealTimeSTTNoTranslateDelegate.Clear();
	RealTimeSTTTranslateDelegate.Clear();


	UXGXunFeiAudioCaptureSubsystem* XunFeiAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGXunFeiAudioCaptureSubsystem>();

	if (XunFeiAudioCaptureSubsystem)
	{
		XunFeiAudioCaptureSubsystem->StopCapturingAudio();
	}

	RealeaseVoiceConsumeRunnable();

	EndSendVoiceData();



}

void UXGXunFeiRealTimeSTTSubsystem::CallInitRealTimeSTTDelegate(bool bInitResult, FString InitMessage)
{
	FXGXunFeiInitRealTimeSTTDelegate TempRealTimeSTTDelegate = InitRealTimeSTTDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempRealTimeSTTDelegate.ExecuteIfBound(bInitResult, InitMessage);

		});



}

void UXGXunFeiRealTimeSTTSubsystem::CallRealTimeSTTNoTranslateDelegate(FString InSrcText)

{

	FXGXunFeiRealTimeSTTNoTranslateDelegate TempRealTimeSTTNoTranslateDelegate = RealTimeSTTNoTranslateDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempRealTimeSTTNoTranslateDelegate.ExecuteIfBound(InSrcText);

		});

}

void UXGXunFeiRealTimeSTTSubsystem::CallRealTimeSTTTranslateDelegate(FString InSrcText, FString InDstText)
{
	FXGXunFeiRealTimeSTTTranslateDelegate TempRealTimeSTTTranslateDelegate = RealTimeSTTTranslateDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempRealTimeSTTTranslateDelegate.ExecuteIfBound(InSrcText, InDstText);

		});


}


void UXGXunFeiRealTimeSTTSubsystem::SendVoiceData(const TArray<float>& InVoiceData)
{

	TArray<int16> ToChangeAuidoData;
	TArray<uint8> BinaryDataToSend;

	for (int32 VoiceIndex = 0; VoiceIndex < 1024; VoiceIndex++)
	{
		ToChangeAuidoData.Add((int16)FMath::Clamp<int32>(FMath::FloorToInt(32767.0f * InVoiceData[VoiceIndex]), -32768, 32767));
		uint8 Bytes[2];
		Bytes[0] = (uint8)(ToChangeAuidoData[VoiceIndex] & 0xFF);
		Bytes[1] = (uint8)((ToChangeAuidoData[VoiceIndex] >> 8) & 0xFF);
		BinaryDataToSend.Add(Bytes[0]);
		BinaryDataToSend.Add(Bytes[1]);
	}

	FScopeLock Lock(&SocketCriticalSection);

	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Send(BinaryDataToSend.GetData(), BinaryDataToSend.Num(), true);

	}




}

void UXGXunFeiRealTimeSTTSubsystem::RealeaseVoiceConsumeRunnable()
{
	if (ConsumeVoiceRunnable.IsValid())
	{
		ConsumeVoiceRunnable->Stop();
	}
}

void UXGXunFeiRealTimeSTTSubsystem::EndSendVoiceData()
{
	FScopeLock Lock(&SocketCriticalSection);

	if (Socket.IsValid() && Socket->IsConnected())
	{
		FString EndStr = TEXT("{\"end\": true}");

		//const char* CharValue = TCHAR_TO_UTF8(*EndStr);

		int32 Length = strlen(TCHAR_TO_UTF8(*EndStr));

		Socket->Send(TCHAR_TO_UTF8(*EndStr), Length, true);
		Socket->Close();
		Socket.Reset();
	}

}



void UXGXunFeiRealTimeSTTSubsystem::OnConnected()
{
	UE_LOG(LogXGXunFeiSTT, Display, TEXT("[%s]:Connect Succeess!"), *FString(__FUNCTION__));

}

void UXGXunFeiRealTimeSTTSubsystem::OnConnectionError(const FString& ErrorMessage)
{
	UE_LOG(LogXGXunFeiSTT, Error, TEXT("[%s]:ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *ErrorMessage);


	FString Message = FString(__FUNCTION__) + TEXT("-ConnectError,Message:") + ErrorMessage;

	XGStopRealTimeSpeechToText();

	Socket.Reset();

	ReakTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;


	CallInitRealTimeSTTDelegate(false, Message);

}

void UXGXunFeiRealTimeSTTSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogXGXunFeiSTT, Warning, TEXT("[%s]:ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));

	XGStopRealTimeSpeechToText();

	Socket.Reset();

	ReakTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;

}

void UXGXunFeiRealTimeSTTSubsystem::OnMessage(const FString& Message)
{
	//UE_LOG(LogXGXunFeiSTT, Display, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *Message);

	FXGXunFeiRealTImeSTTRespInfo RealTImeSTTRespInfo;
	if (!Message.IsEmpty() && FJsonObjectConverter::JsonObjectStringToUStruct(Message, &RealTImeSTTRespInfo))
	{
		if (RealTImeSTTRespInfo.action.Equals(TEXT("started")))
		{
			if (ReakTimeSTTStatus != EXGXunFeiRealTimeSTTStatus::Init)
			{
				FString StartedErroeMessage = TEXT("[%s]:Unknow Procedure Error");

				UE_LOG(LogXGXunFeiSTT, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *StartedErroeMessage);

				CallInitRealTimeSTTDelegate(false, StartedErroeMessage);

				XGStopRealTimeSpeechToText();

				return;

			}

			ReakTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Processing;

			UXGXunFeiAudioCaptureSubsystem* XunFeiAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGXunFeiAudioCaptureSubsystem>();
			FAudioCaptureDeviceInfo AudioCaptureDeviceInfo;

			XunFeiAudioCaptureSubsystem->StartCapturingAudio();

			bool bGetAudioDevice = XunFeiAudioCaptureSubsystem->GetAudioCaptureDeviceInfo(AudioCaptureDeviceInfo);

			bool bRightAudioRateAndChannles = AudioCaptureDeviceInfo.SampleRate == 48000 && AudioCaptureDeviceInfo.NumInputChannels == 2;

			bRightAudioRateAndChannles = true;

			if (!bGetAudioDevice || !bRightAudioRateAndChannles)
			{
				FString AudioErrorMessage = TEXT("AudioCaptureDevice is not right!");

				UE_LOG(LogXGXunFeiSTT, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *AudioErrorMessage);

				XunFeiAudioCaptureSubsystem->StopCapturingAudio();

				CallInitRealTimeSTTDelegate(false, AudioErrorMessage);

				XGStopRealTimeSpeechToText();

				return;
			}

			UE_LOG(LogXGXunFeiSTT, Display, TEXT("[%s]:Audio DeviceName:[%s],NumInputChannels:[%d],SampleRate:[%d]"),
				*FString(__FUNCTION__),
				*(AudioCaptureDeviceInfo.DeviceName.ToString()),
				AudioCaptureDeviceInfo.NumInputChannels,
				AudioCaptureDeviceInfo.SampleRate);


			ConsumeVoiceRunnable = MakeShared<FXGXunFeiConsumeVoiceRunnable>(TEXT("XunFeiConsumeVoiceRunnable"), this);
			FRunnableThread* RunnableThread = FRunnableThread::Create(ConsumeVoiceRunnable.Get(), *(ConsumeVoiceRunnable->ThreadName));

			FString InitMessage = TEXT("XunFeiRealTimeSTT Init Success !");

			CallInitRealTimeSTTDelegate(true, InitMessage);

			UE_LOG(LogXGXunFeiSTT, Display, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *InitMessage);

		}
		else if (RealTImeSTTRespInfo.action.Equals(TEXT("error")))
		{
			UE_LOG(LogXGXunFeiSTT, Error, TEXT("[%s]--Error--[%s]"), *FString(__FUNCTION__), *(RealTImeSTTRespInfo.desc));

			CallInitRealTimeSTTDelegate(false, *(RealTImeSTTRespInfo.desc));

			XGStopRealTimeSpeechToText();

			return;

		}
		else if (RealTImeSTTRespInfo.action.Equals(TEXT("result")))
		{
			TSharedPtr<FJsonObject> ResultDataObj;
			TSharedRef<TJsonReader<>> DataReader = TJsonReaderFactory<>::Create(RealTImeSTTRespInfo.data);
			FJsonSerializer::Deserialize(DataReader, ResultDataObj);

			bool bFindDiz = ResultDataObj->HasField(TEXT("biz"));

			if (bFindDiz)
			{
				FXGXunFeiRealTimeSTTTranslateData RealTimeSTTTranslateData;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(RealTImeSTTRespInfo.data, &RealTimeSTTTranslateData))
				{
					if (RealTimeSTTTranslateData.ed > 0)
					{
						FString dst = RealTimeSTTTranslateData.dst;
						FString src = RealTimeSTTTranslateData.src;

						UE_LOG(LogXGXunFeiSTT, Log, TEXT("[%s]: [dst]-[%s]||[src]-[%s]"), *FString(__FUNCTION__), *dst, *src);

						CallRealTimeSTTTranslateDelegate(src, dst);


					}
				}

			}
			else
			{

				FXGXunFeiRealTimeSTTNoTranslateData RealTimeSTTNoTranslateData;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(RealTImeSTTRespInfo.data, &RealTimeSTTNoTranslateData))
				{

					bool bFinal = !RealTimeSTTNoTranslateData.cn.st.ed.Equals(TEXT("0"));

					if (bFinal)
					{
						TArray<FXGXunFeiRealTimeSTTNoTranslateWSData> WSDatas = RealTimeSTTNoTranslateData.cn.st.rt[0].ws;

						FString OutText;

						for (auto& TmpWs : WSDatas)
						{
							for (auto& TmpCW : TmpWs.cw)
							{
								OutText += TmpCW.w;
							}
						}

						UE_LOG(LogXGXunFeiSTT, Log, TEXT("[%s]:[src]-[%s] "), *FString(__FUNCTION__), *OutText);
						CallRealTimeSTTNoTranslateDelegate(OutText);
					}


				}

			}

		}

	}

}

void UXGXunFeiRealTimeSTTSubsystem::OnMessageSent(const FString& MessageString)
{


}


