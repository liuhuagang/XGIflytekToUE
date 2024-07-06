// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiRealTimeSTTSubsystem.h"

#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "JsonObjectConverter.h"
#include "HAL/RunnableThread.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/Base64.h"

#include "LogXGXunFeiSTT.h"
#include "XGXunFeiAudioCaptureSubsystem.h"
#include "XGXunFeiConsumeVoiceRunnable.h"



UXGXunFeiRealTimeSTTSubsystem* UXGXunFeiRealTimeSTTSubsystem::RealTimeSTTSubsystemPtr =nullptr;

bool UXGXunFeiRealTimeSTTSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGXunFeiRealTimeSTTSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	Super::Initialize(Collection);

	RealTimeSTTSubsystemPtr = this;
}

void UXGXunFeiRealTimeSTTSubsystem::Deinitialize()
{
	XGForceToStopRealTimeSPeechToText(true);

	RealTimeSTTSubsystemPtr = nullptr;

	Super::Deinitialize();

}


bool UXGXunFeiRealTimeSTTSubsystem::IsRealTimeSTTReady()
{
	return RealTimeSTTStatus == EXGXunFeiRealTimeSTTStatus::Ready;
}

FGuid UXGXunFeiRealTimeSTTSubsystem::XGBeginRealTimeSpeechToText(
	FString InSTTAppID,
	FString InSTTAPIKey,
	FXGXunFeiRealTimeSTTReqInfo& InRealTimeSTTReqInfo,
	FXGXunFeiInitRealTimeSTTDelegate InRealTimeSTTInitDelegate,
	FXGXunFeiRealTimeSTTRespDelegate InRealTimeSTTRespDelegate,
	FXGXunFeiRealTimeSTTCloseDelegate InRealTimeSTTCloseDelegate)
{
	STTAsyncID = FGuid::NewGuid();

	RealTimeSTTInitDelegate = InRealTimeSTTInitDelegate;

	if (!IsRealTimeSTTReady())
	{

		CallInitRealTimeSTTDelegate(STTAsyncID, false, TEXT("XGXunFeiRealTimeSTT is Running !"), FXGXunFeiRealTimeSTTInitInformation());

		RealTimeSTTInitDelegate.Clear();


		UE_LOG(LogXGXunFeiRealTimeSTT, Warning, TEXT("[%s]-AsyncID:[%s] try to connect to iFLyTek,But It is Running"), *FString(__FUNCTION__), *(STTAsyncID.ToString()));

		return STTAsyncID;

	}



	UE_LOG(LogXGXunFeiRealTimeSTT, Display, TEXT("[%s]-AsyncID:[%s] is trying to connect to iFLyTek"), *FString(__FUNCTION__), *(STTAsyncID.ToString()));


	RealTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Init;

	STTAppID = InSTTAppID;

	STTAPIKey = InSTTAPIKey;


	RealTimeSTTRespDelegate = InRealTimeSTTRespDelegate;

	RealTimeSTTCloseDelegate = InRealTimeSTTCloseDelegate;

	FModuleManager::Get().LoadModuleChecked("WebSockets");
	FString ServerProtocol = TEXT("wss");
	FString ServerURL = TEXT("wss://rtasr.xfyun.cn/v1/ws?");

	ServerURL += InRealTimeSTTReqInfo.GenerateRequireParams(STTAppID, STTAPIKey);


	AsyncTask(ENamedThreads::GameThread, [ServerURL, ServerProtocol, this]() {

		FModuleManager::Get().LoadModuleChecked("WebSockets");
		Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
		Socket->OnConnected().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnConnected);
		Socket->OnConnectionError().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnConnectionError);
		Socket->OnClosed().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnClosed);
		Socket->OnMessage().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnMessage);
		Socket->OnMessageSent().AddUObject(this, &UXGXunFeiRealTimeSTTSubsystem::OnMessageSent);

		Socket->Connect();
		});


	return STTAsyncID;

}

void UXGXunFeiRealTimeSTTSubsystem::XGStopRealTimeSpeechToText()
{
	if (RealTimeSTTStatus == EXGXunFeiRealTimeSTTStatus::Ready)
	{
		UE_LOG(LogXGXunFeiRealTimeSTT, Warning, TEXT("[%s]-AsyncID:[%s] try to close iFLyTek connect,but It was closed"), *FString(__FUNCTION__), *(STTAsyncID.ToString()));
		return;
	}

	RealTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::WaitToServerClose;

	UE_LOG(LogXGXunFeiRealTimeSTT, Warning, TEXT("[%s]-AsyncID:[%s] try to close iFLyTek Connect"), *FString(__FUNCTION__), *(STTAsyncID.ToString()));

	RealseaseVoiceGenerateRunnale();

	RealeaseVoiceConsumeRunnable();

	EndSendVoiceData();



}

void UXGXunFeiRealTimeSTTSubsystem::XGForceToStopRealTimeSPeechToText(bool IsDeinitialize)
{
	if (RealTimeSTTStatus == EXGXunFeiRealTimeSTTStatus::Ready)
	{
		if (IsDeinitialize)
		{

	
		}
		else
		{
			UE_LOG(LogXGXunFeiRealTimeSTT, Warning, TEXT("[%s]-AsyncID:[%s] try to force to close iFLyTek connect,but It was closed"), *FString(__FUNCTION__), *(STTAsyncID.ToString()));
		}

	}
	else
	{

		UE_LOG(LogXGXunFeiRealTimeSTT, Warning, TEXT("[%s]-AsyncID:[%s]  force to close iFLyTek connect."), *FString(__FUNCTION__), *(STTAsyncID.ToString()));


		RealseaseVoiceGenerateRunnale();

		RealeaseVoiceConsumeRunnable();

		EndSendVoiceData();

		RealeaseWebSocket();

		RealeaseSTTInfo();

		RealTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;

	}










}

void UXGXunFeiRealTimeSTTSubsystem::CallInitRealTimeSTTDelegate(FGuid InSTTAsyncID, bool bInInitResult, FString InInitRespMessage, FXGXunFeiRealTimeSTTInitInformation InRealTimeSTTInitInformation)
{
	FXGXunFeiInitRealTimeSTTDelegate TempRealTimeSTTDelegate = RealTimeSTTInitDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempRealTimeSTTDelegate.ExecuteIfBound(InSTTAsyncID, bInInitResult, InInitRespMessage, InRealTimeSTTInitInformation);

		});
}

void UXGXunFeiRealTimeSTTSubsystem::CallRealTimeSTTRespDelegate(FXGXunFeiRealTimeSTTInformation InRealTimeSTTInformation)
{
	FXGXunFeiRealTimeSTTRespDelegate TempRealTimeSTTDelegate = RealTimeSTTRespDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempRealTimeSTTDelegate.ExecuteIfBound(InRealTimeSTTInformation);

		});
}

void UXGXunFeiRealTimeSTTSubsystem::CallRealTimeSTTCloseDelegate(FGuid InSTTAsyncID, int32 InStatusCode, FString InReason, bool bInWasClean)
{
	FXGXunFeiRealTimeSTTCloseDelegate TempRealTimeSTTDelegate = RealTimeSTTCloseDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempRealTimeSTTDelegate.ExecuteIfBound(InSTTAsyncID, InStatusCode, InReason, bInWasClean);

		});
}

void UXGXunFeiRealTimeSTTSubsystem::SendVoiceData(const TArray<float>& InVoiceData)
{

	TArray<int16> ToChangeAuidoData;

	TArray<uint8> BinaryDataToSend;

	for (int32 VoiceIndex = 0; VoiceIndex < InVoiceData.Num(); VoiceIndex++)
	{
		ToChangeAuidoData.Add((int16)FMath::Clamp<int32>(FMath::FloorToInt(32767.0f * InVoiceData[VoiceIndex]), -32768, 32767));
		uint8 Bytes[2];
		Bytes[0] = (uint8)(ToChangeAuidoData[VoiceIndex] & 0xFF);
		Bytes[1] = (uint8)((ToChangeAuidoData[VoiceIndex] >> 8) & 0xFF);
		BinaryDataToSend.Add(Bytes[0]);
		BinaryDataToSend.Add(Bytes[1]);
	}

	AsyncTask(ENamedThreads::GameThread,[BinaryDataToSend](){
	
		if (UXGXunFeiRealTimeSTTSubsystem::RealTimeSTTSubsystemPtr&& UXGXunFeiRealTimeSTTSubsystem::RealTimeSTTSubsystemPtr->RealTimeSTTStatus == EXGXunFeiRealTimeSTTStatus::Processing)
		{
			if (RealTimeSTTSubsystemPtr->Socket.IsValid() && RealTimeSTTSubsystemPtr->Socket->IsConnected())
			{
				RealTimeSTTSubsystemPtr->Socket->Send(BinaryDataToSend.GetData(), BinaryDataToSend.Num(), true);

			}
		}

	});






}

void UXGXunFeiRealTimeSTTSubsystem::RealseaseVoiceGenerateRunnale()
{
	UXGXunFeiAudioCaptureSubsystem* XunFeiAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGXunFeiAudioCaptureSubsystem>();

	if (XunFeiAudioCaptureSubsystem)
	{
		XunFeiAudioCaptureSubsystem->StopCapturingAudio();
	}
}

void UXGXunFeiRealTimeSTTSubsystem::RealeaseVoiceConsumeRunnable()
{

	if (ConsumeVoiceRunnable.IsValid())
	{
		ConsumeVoiceRunnable->Stop();
	}

	if (RunnableThread)
	{
		RunnableThread->WaitForCompletion();
		delete RunnableThread;
		RunnableThread = nullptr;
	}

}

void UXGXunFeiRealTimeSTTSubsystem::RealeaseWebSocket()
{

	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}

	Socket.Reset();

}

void UXGXunFeiRealTimeSTTSubsystem::RealeaseSTTInfo()
{
	STTAppID = TEXT("");

	STTAPIKey = TEXT("");

	STTAsyncID = FGuid();

	RealTimeSTTInitDelegate.Clear();

	RealTimeSTTRespDelegate.Clear();

	RealTimeSTTCloseDelegate.Clear();
}

void UXGXunFeiRealTimeSTTSubsystem::EndSendVoiceData()
{

	if (Socket.IsValid() && Socket->IsConnected())
	{
		FString EndStr = TEXT("{\"end\": true}");

		int32 Length = strlen(TCHAR_TO_UTF8(*EndStr));

		Socket->Send(TCHAR_TO_UTF8(*EndStr), Length, true);
	}


}

void UXGXunFeiRealTimeSTTSubsystem::OnConnected()
{
	UE_LOG(LogXGXunFeiRealTimeSTT, Display, TEXT("[%s]-AsyncID:[%s] Connect Succeess!"), *FString(__FUNCTION__), *(STTAsyncID.ToString()));

}

void UXGXunFeiRealTimeSTTSubsystem::OnConnectionError(const FString& ErrorMessage)
{
	UE_LOG(LogXGXunFeiRealTimeSTT, Error, TEXT("[%s]-AsyncID:[%s] ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *(STTAsyncID.ToString()), *ErrorMessage);

	RealeaseWebSocket();

	RealTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;

	CallInitRealTimeSTTDelegate(STTAsyncID, false, ErrorMessage, FXGXunFeiRealTimeSTTInitInformation());

	RealeaseSTTInfo();


}

void UXGXunFeiRealTimeSTTSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogXGXunFeiRealTimeSTT, Warning, TEXT("[%s]-AsyncID:[%s] ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), *(STTAsyncID.ToString()), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));

	RealseaseVoiceGenerateRunnale();

	RealeaseVoiceConsumeRunnable();

	RealeaseWebSocket();

	RealTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;

	CallRealTimeSTTCloseDelegate(STTAsyncID, StatusCode, Reason, bWasClean);

	RealeaseSTTInfo();

}

void UXGXunFeiRealTimeSTTSubsystem::OnMessage(const FString& Message)
{
	UE_LOG(LogXGXunFeiRealTimeSTT, Verbose, TEXT("[%s]-AsyncID:[%s], Message:%s"), *(STTAsyncID.ToString()), *FString(__FUNCTION__), *Message);

	FXGXunFeiRealTImeSTTRespInfo RealTImeSTTRespInfo;

	if (!Message.IsEmpty() && FJsonObjectConverter::JsonObjectStringToUStruct(Message, &RealTImeSTTRespInfo,0,0))
	{
		if (RealTImeSTTRespInfo.action.Equals(TEXT("started")))
		{
			if (RealTimeSTTStatus != EXGXunFeiRealTimeSTTStatus::Init)
			{

				UE_LOG(LogXGXunFeiRealTimeSTT, Error, TEXT("[%s]-AsyncID:[%s] Unknow Procedure Error"), *FString(__FUNCTION__), *(STTAsyncID.ToString()));

				FXGXunFeiRealTimeSTTInitInformation  RealTimeSTTInitInformation;

				RealTimeSTTInitInformation.RealTImeSTTInitInfo = RealTImeSTTRespInfo;

				CallInitRealTimeSTTDelegate(STTAsyncID, false, TEXT("Unknow Procedure Error"), RealTimeSTTInitInformation);

				XGForceToStopRealTimeSPeechToText();

				return;

			}

			RealTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Processing;

			UXGXunFeiAudioCaptureSubsystem* XunFeiAudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UXGXunFeiAudioCaptureSubsystem>();
			FAudioCaptureDeviceInfo AudioCaptureDeviceInfo;

			XunFeiAudioCaptureSubsystem->StartCapturingAudio();

			bool bGetAudioDevice = XunFeiAudioCaptureSubsystem->GetAudioCaptureDeviceInfo(AudioCaptureDeviceInfo);

			bool bRightAudioRateAndChannles = AudioCaptureDeviceInfo.SampleRate == 48000 && AudioCaptureDeviceInfo.NumInputChannels == 2;

			bRightAudioRateAndChannles = true;

			if (!bGetAudioDevice || !bRightAudioRateAndChannles)
			{
				FString AudioErrorMessage = TEXT("AudioCaptureDevice is not right!");

				UE_LOG(LogXGXunFeiRealTimeSTT, Error, TEXT("[%s]-AsyncID:[%s]--[%s]"), *FString(__FUNCTION__), *(STTAsyncID.ToString()), *AudioErrorMessage);

				XunFeiAudioCaptureSubsystem->StopCapturingAudio();

				FXGXunFeiRealTimeSTTInitInformation  RealTimeSTTInitInformation;

				RealTimeSTTInitInformation.RealTImeSTTInitInfo = RealTImeSTTRespInfo;

				CallInitRealTimeSTTDelegate(STTAsyncID, false, AudioErrorMessage, RealTimeSTTInitInformation);

				XGForceToStopRealTimeSPeechToText();

				return;
			}

			UE_LOG(LogXGXunFeiRealTimeSTT, Warning, TEXT("[%s]-AsyncID:[%s]:Audio DeviceName:[%s],NumInputChannels:[%d],SampleRate:[%d]"),
				*FString(__FUNCTION__),
				*(STTAsyncID.ToString()),
				*(AudioCaptureDeviceInfo.DeviceName.ToString()),
				AudioCaptureDeviceInfo.NumInputChannels,
				AudioCaptureDeviceInfo.SampleRate);


			ConsumeVoiceRunnable = MakeShared<FXGXunFeiConsumeVoiceRunnable>(TEXT("XunFeiConsumeVoiceRunnable"), EXGXunFeiConsumeVoiceType::RealtimeSTT);

			RunnableThread = FRunnableThread::Create(ConsumeVoiceRunnable.Get(), *(ConsumeVoiceRunnable->ThreadName));

			FString InitMessage = TEXT("XunFeiRealTimeSTT Init Success !");

			FXGXunFeiRealTimeSTTInitInformation  RealTimeSTTInitInformation;

			RealTimeSTTInitInformation.RealTImeSTTInitInfo = RealTImeSTTRespInfo;

			CallInitRealTimeSTTDelegate(STTAsyncID, true, InitMessage, RealTimeSTTInitInformation);

			UE_LOG(LogXGXunFeiRealTimeSTT, Display, TEXT("[%s]-AsyncID:[%s]--[%s]"), *FString(__FUNCTION__), *(STTAsyncID.ToString()), *InitMessage);

		}
		else if (RealTImeSTTRespInfo.action.Equals(TEXT("error")))
		{
			UE_LOG(LogXGXunFeiRealTimeSTT, Error, TEXT("[%s]-AsyncID:[%s]--Error--[%s]"), *FString(__FUNCTION__), *(STTAsyncID.ToString()), *(RealTImeSTTRespInfo.desc));

			FXGXunFeiRealTimeSTTInitInformation  RealTimeSTTInitInformation;

			RealTimeSTTInitInformation.RealTImeSTTInitInfo = RealTImeSTTRespInfo;

			CallInitRealTimeSTTDelegate(STTAsyncID, false, *(RealTImeSTTRespInfo.desc), RealTimeSTTInitInformation);

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
				if (FJsonObjectConverter::JsonObjectStringToUStruct(RealTImeSTTRespInfo.data, &RealTimeSTTTranslateData,0,0))
				{
					if (RealTimeSTTTranslateData.ed > 0)
					{
						FString Dst = RealTimeSTTTranslateData.dst;
						FString Src = RealTimeSTTTranslateData.src;

						UE_LOG(LogXGXunFeiRealTimeSTT, Log, TEXT("[%s]-AsyncID [%s]: [dst]-[%s]||[src]-[%s]"), *FString(__FUNCTION__), *(STTAsyncID.ToString()),*Dst, *Src);

						FXGXunFeiRealTimeSTTInformation RealTimeSTTInformation;

						RealTimeSTTInformation.AsyncSTTID = STTAsyncID;
						RealTimeSTTInformation.bSuccess = true;
						RealTimeSTTInformation.RespType = EXGXunFeiRealTimeSTTRespType::FinalTextWithSrcAndDst;
						RealTimeSTTInformation.SrcText = Src;
						RealTimeSTTInformation.DstText = Dst;

						RealTimeSTTInformation.RawInformation.RealTImeSTTRespInfo = RealTImeSTTRespInfo;
						RealTimeSTTInformation.RawInformation.RealTimeSTTTranslateData = RealTimeSTTTranslateData;

						CallRealTimeSTTRespDelegate(RealTimeSTTInformation);


					}
				}

			}
			else
			{

				FXGXunFeiRealTimeSTTNoTranslateData RealTimeSTTNoTranslateData;

				if (FJsonObjectConverter::JsonObjectStringToUStruct(RealTImeSTTRespInfo.data, &RealTimeSTTNoTranslateData,0,0))
				{

					bool bMiddle = RealTimeSTTNoTranslateData.cn.st.type.Equals(TEXT("1"));

					if (bMiddle)
					{

						TArray<FXGXunFeiRealTimeSTTNoTranslateWSData> WSDatas = RealTimeSTTNoTranslateData.cn.st.rt[0].ws;

						FString OutText = TEXT("");
						FString OutRoleType=TEXT("0");
						for (auto& TmpWs : WSDatas)
						{
							for (auto& TmpCW : TmpWs.cw)
							{
								OutText += TmpCW.w;

								if (!TmpCW.rl.Equals(TEXT("0")))
								{
									OutRoleType= TmpCW.rl;
								}

								
							}
						}



						FXGXunFeiRealTimeSTTInformation RealTimeSTTInformation;

						RealTimeSTTInformation.AsyncSTTID = STTAsyncID;
						RealTimeSTTInformation.bSuccess = true;
						RealTimeSTTInformation.RespType = EXGXunFeiRealTimeSTTRespType::MiddleTextOnlySrc;
						RealTimeSTTInformation.SrcText = OutText;
						RealTimeSTTInformation.RoleType = OutRoleType;

						RealTimeSTTInformation.RawInformation.RealTImeSTTRespInfo = RealTImeSTTRespInfo;
						RealTimeSTTInformation.RawInformation.RealTimeSTTNoTranslateData = RealTimeSTTNoTranslateData;

						CallRealTimeSTTRespDelegate(RealTimeSTTInformation);

					}
					else
					{
						TArray<FXGXunFeiRealTimeSTTNoTranslateWSData> WSDatas = RealTimeSTTNoTranslateData.cn.st.rt[0].ws;

						FString OutText;
						FString OutRoleType = TEXT("0");

						for (auto& TmpWs : WSDatas)
						{
							for (auto& TmpCW : TmpWs.cw)
							{
								OutText += TmpCW.w;

								if (!TmpCW.rl.Equals(TEXT("0")))
								{
									OutRoleType = TmpCW.rl;
								}

							}
						}

						UE_LOG(LogXGXunFeiRealTimeSTT, Verbose, TEXT("[%s]-AsyncID-[%s]:[src]-[%s] "), *FString(__FUNCTION__), *(STTAsyncID.ToString()), *OutText);

						FXGXunFeiRealTimeSTTInformation RealTimeSTTInformation;

						RealTimeSTTInformation.AsyncSTTID = STTAsyncID;
						RealTimeSTTInformation.bSuccess = true;
						RealTimeSTTInformation.RespType = EXGXunFeiRealTimeSTTRespType::FinalTextOnlySrc;
						RealTimeSTTInformation.SrcText = OutText;
						RealTimeSTTInformation.RoleType = OutRoleType;

						RealTimeSTTInformation.RawInformation.RealTImeSTTRespInfo = RealTImeSTTRespInfo;
						RealTimeSTTInformation.RawInformation.RealTimeSTTNoTranslateData = RealTimeSTTNoTranslateData;

						CallRealTimeSTTRespDelegate(RealTimeSTTInformation);

					}


				}

			}

		}

	}

}

void UXGXunFeiRealTimeSTTSubsystem::OnMessageSent(const FString& MessageString)
{
	//UE_LOG(LogXGXunFeiRealTimeSTT, Display, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *MessageString);


}


