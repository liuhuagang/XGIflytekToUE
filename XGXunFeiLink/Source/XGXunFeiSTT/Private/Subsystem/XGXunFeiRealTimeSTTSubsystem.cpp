// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiRealTimeSTTSubsystem.h"

#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "JsonObjectConverter.h"
#include "HAL/RunnableThread.h"


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
		return;
	}


	InitRealTimeSTTDelegate = InitRealTimeSTTDelegate;
	RealTimeSTTNoTranslateDelegate = InRealTimeSTTNoTranslateDelegate;
	RealTimeSTTTranslateDelegate = InRealTimeSTTTranslateDelegate;

	FModuleManager::Get().LoadModuleChecked("WebSockets");
	FString ServerProtocol = TEXT("ws");
	FString ServerURL = TEXT("ws://rtasr.xfyun.cn/v1/ws?");

	ServerURL += InRealTimeSTTReqInfo.GenerateRequireParams();
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	Socket->OnConnected().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnMessageSent);

	Socket->Connect();

}

void UXGXunFeiRealTimeSTTSubsystem::XGStopRealTimeSpeechToText()
{
	if (ReakTimeSTTStatus == EXGXunFeiRealTimeSTTStatus::Ready)
	{
		return;
	}

	InitRealTimeSTTDelegate.Clear();
	RealTimeSTTNoTranslateDelegate.Clear();
	RealTimeSTTTranslateDelegate.Clear();


	UXGXunFeiAudioCaptureSubsystem* XunFeiAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGXunFeiAudioCaptureSubsystem>();

	XunFeiAudioCaptureSubsystem->StopCapturingAudio();

	RealeaseVoiceConsumeRunnable();

	EndSendVoiceData();

	ReakTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;
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
		const char* CharValue = TCHAR_TO_UTF8(*EndStr);
		int32 Length = strlen(CharValue);
		Socket->Send(CharValue, Length, true);
		Socket->Close();
	}
	Socket.Reset();
}



void UXGXunFeiRealTimeSTTSubsystem::OnConnected()
{
	UE_LOG(LogXGXunFeiSTT, Display, TEXT("[%s]:Connect Succeess!"), *FString(__FUNCTION__));
}

void UXGXunFeiRealTimeSTTSubsystem::OnConnectionError(const FString& ErrorMessage)
{
	UE_LOG(LogXGXunFeiSTT, Error, TEXT("[%s]:ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *ErrorMessage);

	XGStopRealTimeSpeechToText();

	FString Message = FString(__FUNCTION__) + TEXT("-ConnectError,Message:") + ErrorMessage;
	InitRealTimeSTTDelegate.ExecuteIfBound(false, Message);

}

void UXGXunFeiRealTimeSTTSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogXGXunFeiSTT, Warning, TEXT("[%s]:ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));

	XGStopRealTimeSpeechToText();

}

void UXGXunFeiRealTimeSTTSubsystem::OnMessage(const FString& Message)
{
	//UE_LOG(LogXGXunFeiSTT, Display, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *Message);

	FXGXunFeiRealTImeSTTRespInfo RealTImeSTTRespInfo;
	if (!Message.IsEmpty() && FJsonObjectConverter::JsonObjectStringToUStruct(Message, &RealTImeSTTRespInfo))
	{


		if (RealTImeSTTRespInfo.action.Equals(TEXT("started")))
		{
			ReakTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Processing;

			UXGXunFeiAudioCaptureSubsystem* XunFeiAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGXunFeiAudioCaptureSubsystem>();
			FAudioCaptureDeviceInfo AudioCaptureDeviceInfo;

			XunFeiAudioCaptureSubsystem->StartCapturingAudio();

			bool bGetAudioDevice = XunFeiAudioCaptureSubsystem->GetAudioCaptureDeviceInfo(AudioCaptureDeviceInfo);
			bool bRightAudioRateAndChannles = AudioCaptureDeviceInfo.SampleRate == 48000 && AudioCaptureDeviceInfo.NumInputChannels == 2;
			bRightAudioRateAndChannles = true;

			if (!bGetAudioDevice || !bRightAudioRateAndChannles)
			{
				UE_LOG(LogXGXunFeiSTT, Error, TEXT("AudioCaptureDevice is not right!"));
				XunFeiAudioCaptureSubsystem->StopCapturingAudio();

				InitRealTimeSTTDelegate.ExecuteIfBound(false, TEXT("AudioCaptureDevice is not right!"));

				XGStopRealTimeSpeechToText();

				return;
			}



			ConsumeVoiceRunnable = MakeShared<FXGXunFeiConsumeVoiceRunnable>(TEXT("XunFeiConsumeVoiceRunnable"), XunFeiAudioCaptureSubsystem, this);
			FRunnableThread* RunnableThread = FRunnableThread::Create(ConsumeVoiceRunnable.Get(), *(ConsumeVoiceRunnable->ThreadName));

			InitRealTimeSTTDelegate.ExecuteIfBound(true, TEXT("XunFeiRealTimeSTT Init Success !"));


		}
		else if (RealTImeSTTRespInfo.action.Equals(TEXT("error")))
		{
			UE_LOG(LogXGXunFeiSTT, Error, TEXT("[%s]--Error--[%s]"), *FString(__FUNCTION__), *(RealTImeSTTRespInfo.desc));
			XGStopRealTimeSpeechToText();
			InitRealTimeSTTDelegate.ExecuteIfBound(false, *(RealTImeSTTRespInfo.desc));

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

						UE_LOG(LogXGXunFeiSTT, Log, TEXT("[dst] :%s Message [src]:%s"), *dst, *src);
						RealTimeSTTTranslateDelegate.ExecuteIfBound(src, dst);
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

						UE_LOG(LogXGXunFeiSTT, Log, TEXT("[src] :%s "), *OutText);
						RealTimeSTTNoTranslateDelegate.ExecuteIfBound(OutText);
					}


				}

			}

		}

	}

}

void UXGXunFeiRealTimeSTTSubsystem::OnMessageSent(const FString& MessageString)
{


}


