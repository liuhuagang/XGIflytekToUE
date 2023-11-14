// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGiFlyTekRealTimeSTTSubsystem.h"

#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "JsonObjectConverter.h"
#include "HAL/RunnableThread.h"


#include "LogXGiFlyTekSTT.h"
#include "XGiFlyTekRealTimeSTTRespType.h"
#include "XGiFlyTekAudioCaptureSubsystem.h"
#include "XGiFlyTekConsumeVoiceRunnable.h"

bool UXGiFlyTekRealTimeSTTSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGiFlyTekRealTimeSTTSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UXGiFlyTekRealTimeSTTSubsystem::Deinitialize()
{
	XGStopRealTimeSpeechToText();
	Super::Deinitialize();
}

void UXGiFlyTekRealTimeSTTSubsystem::XGBeginRealTimeSpeechToText(
		FXGiFlyTekRealTimeSTTReqInfo& InRealTimeSTTReqInfo,
	FXGiFlyTekInitRealTimeSTTDelegate InInitRealTimeSTTDelegate,
	FXGiFlyTekRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate,
	FXGiFlyTekRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate)
{
	if (ReakTimeSTTStatus != EXGiFlyTekRealTimeSTTStatus::Ready)
	{
		InInitRealTimeSTTDelegate.ExecuteIfBound(false, TEXT("XGiFlyTekRealTimeSTT is Running !"));
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

	Socket->OnConnected().AddUObject(this, &UXGiFlyTekRealTimeSTTSubsystem::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UXGiFlyTekRealTimeSTTSubsystem::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UXGiFlyTekRealTimeSTTSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UXGiFlyTekRealTimeSTTSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UXGiFlyTekRealTimeSTTSubsystem::OnMessageSent);

	Socket->Connect();

}

void UXGiFlyTekRealTimeSTTSubsystem::XGStopRealTimeSpeechToText()
{
	if (ReakTimeSTTStatus == EXGiFlyTekRealTimeSTTStatus::Ready)
	{
		return;
	}

	InitRealTimeSTTDelegate.Clear();
	RealTimeSTTNoTranslateDelegate.Clear();
	RealTimeSTTTranslateDelegate.Clear();


	UXGiFlyTekAudioCaptureSubsystem* iFlyTekAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGiFlyTekAudioCaptureSubsystem>();

	iFlyTekAudioCaptureSubsystem->StopCapturingAudio();

	RealeaseVoiceConsumeRunnable();

	EndSendVoiceData();

	ReakTimeSTTStatus = EXGiFlyTekRealTimeSTTStatus::Ready;
}


void UXGiFlyTekRealTimeSTTSubsystem::SendVoiceData(const TArray<float>& InVoiceData)
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

void UXGiFlyTekRealTimeSTTSubsystem::RealeaseVoiceConsumeRunnable()
{
	if (ConsumeVoiceRunnable.IsValid())
	{
		ConsumeVoiceRunnable->Stop();
	}
}

void UXGiFlyTekRealTimeSTTSubsystem::EndSendVoiceData()
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



void UXGiFlyTekRealTimeSTTSubsystem::OnConnected()
{
	UE_LOG(LogXGiFlyTekSTT, Display, TEXT("[%s]:Connect Succeess!"), *FString(__FUNCTION__));
}

void UXGiFlyTekRealTimeSTTSubsystem::OnConnectionError(const FString& ErrorMessage)
{
	UE_LOG(LogXGiFlyTekSTT, Error, TEXT("[%s]:ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *ErrorMessage);

	XGStopRealTimeSpeechToText();

	FString Message = FString(__FUNCTION__) + TEXT("-ConnectError,Message:") + ErrorMessage;
	InitRealTimeSTTDelegate.ExecuteIfBound(false, Message);

}

void UXGiFlyTekRealTimeSTTSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogXGiFlyTekSTT, Warning, TEXT("[%s]:ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));

	XGStopRealTimeSpeechToText();

}

void UXGiFlyTekRealTimeSTTSubsystem::OnMessage(const FString& Message)
{
	//UE_LOG(LogXGiFlyTekSTT, Display, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *Message);

	FXGiFlyTekRealTImeSTTRespInfo RealTImeSTTRespInfo;
	if (!Message.IsEmpty() && FJsonObjectConverter::JsonObjectStringToUStruct(Message, &RealTImeSTTRespInfo))
	{


		if (RealTImeSTTRespInfo.action.Equals(TEXT("started")))
		{
			ReakTimeSTTStatus = EXGiFlyTekRealTimeSTTStatus::Processing;

			UXGiFlyTekAudioCaptureSubsystem* iFlyTekAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGiFlyTekAudioCaptureSubsystem>();
			FAudioCaptureDeviceInfo AudioCaptureDeviceInfo;

			iFlyTekAudioCaptureSubsystem->StartCapturingAudio();

			bool bGetAudioDevice = iFlyTekAudioCaptureSubsystem->GetAudioCaptureDeviceInfo(AudioCaptureDeviceInfo);
			bool bRightAudioRateAndChannles = AudioCaptureDeviceInfo.SampleRate == 48000 && AudioCaptureDeviceInfo.NumInputChannels == 2;
			bRightAudioRateAndChannles = true;

			if (!bGetAudioDevice || !bRightAudioRateAndChannles)
			{
				UE_LOG(LogXGiFlyTekSTT, Error, TEXT("AudioCaptureDevice is not right!"));
				iFlyTekAudioCaptureSubsystem->StopCapturingAudio();

				InitRealTimeSTTDelegate.ExecuteIfBound(false, TEXT("AudioCaptureDevice is not right!"));

				XGStopRealTimeSpeechToText();

				return;
			}



			ConsumeVoiceRunnable = MakeShared<FXGiFlyTekConsumeVoiceRunnable>(TEXT("iFlyTekConsumeVoiceRunnable"), iFlyTekAudioCaptureSubsystem, this);
			FRunnableThread* RunnableThread = FRunnableThread::Create(ConsumeVoiceRunnable.Get(), *(ConsumeVoiceRunnable->ThreadName));

			InitRealTimeSTTDelegate.ExecuteIfBound(true, TEXT("iFlyTekRealTimeSTT Init Success !"));


		}
		else if (RealTImeSTTRespInfo.action.Equals(TEXT("error")))
		{
			UE_LOG(LogXGiFlyTekSTT, Error, TEXT("[%s]--Error--[%s]"), *FString(__FUNCTION__), *(RealTImeSTTRespInfo.desc));
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
				FXGiFlyTekRealTimeSTTTranslateData RealTimeSTTTranslateData;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(RealTImeSTTRespInfo.data, &RealTimeSTTTranslateData))
				{
					if (RealTimeSTTTranslateData.ed > 0)
					{
						FString dst = RealTimeSTTTranslateData.dst;
						FString src = RealTimeSTTTranslateData.src;

						UE_LOG(LogXGiFlyTekSTT, Log, TEXT("[dst] :%s Message [src]:%s"), *dst, *src);
						RealTimeSTTTranslateDelegate.ExecuteIfBound(src, dst);
					}
				}

			}
			else
			{

				FXGiFlyTekRealTimeSTTNoTranslateData RealTimeSTTNoTranslateData;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(RealTImeSTTRespInfo.data, &RealTimeSTTNoTranslateData))
				{

					bool bFinal = !RealTimeSTTNoTranslateData.cn.st.ed.Equals(TEXT("0"));

					if (bFinal)
					{
						TArray<FXGiFlyTekRealTimeSTTNoTranslateWSData> WSDatas = RealTimeSTTNoTranslateData.cn.st.rt[0].ws;

						FString OutText;

						for (auto& TmpWs : WSDatas)
						{
							for (auto& TmpCW : TmpWs.cw)
							{
								OutText += TmpCW.w;
							}
						}

						UE_LOG(LogXGiFlyTekSTT, Log, TEXT("[src] :%s "), *OutText);
						RealTimeSTTNoTranslateDelegate.ExecuteIfBound(OutText);
					}


				}

			}

		}

	}

}

void UXGiFlyTekRealTimeSTTSubsystem::OnMessageSent(const FString& MessageString)
{


}


