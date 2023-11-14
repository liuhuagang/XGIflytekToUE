// Copyright Xiao Gang. All Rights Reserved.

#include "XGiFlyTekTTSAsyncAction.h"

#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "JsonObjectConverter.h"

#include "Misc/Paths.h"
#include "Misc/Base64.h"
#include "Misc/FileHelper.h"

#include "XGiFlyTekCoreSettings.h"
#include "XGiFlyTekCoreBPLibrary.h"
#include "LogXGiFlyTekTTS.h"
#include "XGiFlyTekTTSRespType.h"


UXGiFlyTekTTSAsyncAction::UXGiFlyTekTTSAsyncAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}


UXGiFlyTekTTSAsyncAction* UXGiFlyTekTTSAsyncAction::XGiFlyTekTextToSpeech(
	UObject* WorldContextObject,
	const FString& InText,
	bool bInSaveToLocal,
	const FString& InSaveFileFullPath,
	FXGiFlyTekTTSReqInfo IniFlyTekTTSReqInfo)
{
	UXGiFlyTekTTSAsyncAction* iFlyTekTTSAsyncAction = NewObject<UXGiFlyTekTTSAsyncAction>();

	iFlyTekTTSAsyncAction->SetiFlyTekText(InText);
	iFlyTekTTSAsyncAction->SetIsSaveToLocal(bInSaveToLocal);
	iFlyTekTTSAsyncAction->SetSaveFileFullPath(InSaveFileFullPath);
	iFlyTekTTSAsyncAction->SetiFlyTekTTSReqInfo(IniFlyTekTTSReqInfo);



	iFlyTekTTSAsyncAction->RegisterWithGameInstance(WorldContextObject);


	return iFlyTekTTSAsyncAction;
}
void UXGiFlyTekTTSAsyncAction::Activate()
{
	Super::Activate();

	if (bSaveToLocal)
	{
		FString FileDirectory = FPaths::GetPath(SaveFileFullPath);
		bool bDirectory = FPaths::DirectoryExists(FileDirectory);
		FString FileExtension = FPaths::GetExtension(SaveFileFullPath);
		bool bExtension = FileExtension.Equals(TEXT("wav"));

		if (!bDirectory)
		{
			FString Message = FString::Printf(TEXT("[%s]:FileDirectory don't exist ! FilePath: [%s]"), *FString(__FUNCTION__), *SaveFileFullPath);

			UE_LOG(LogXGiFlyTekTTS, Error, TEXT("%s"), *Message);
			OnFail.Broadcast(false, Message, nullptr);
			RealeaseResources();
		}

		if (!bExtension)
		{
			FString Message = FString::Printf(TEXT("[%s]:FileName is Wrong,it must endwith [.wav] ! FilePath: [%s]"), *FString(__FUNCTION__), *SaveFileFullPath);
			UE_LOG(LogXGiFlyTekTTS, Error, TEXT("%s"), *Message);
			OnFail.Broadcast(false, Message, nullptr);
			RealeaseResources();
		}

	}

	if (iFlyTekText.IsEmpty())
	{
		FString Message = FString::Printf(TEXT("[%s]:InText is Empty,it must be input!"), *FString(__FUNCTION__));
		UE_LOG(LogXGiFlyTekTTS, Error, TEXT("%s"), *Message);
		OnFail.Broadcast(false, Message, nullptr);
		RealeaseResources();
	}

	CreateiFlyTekTTSSocket();

}

void UXGiFlyTekTTSAsyncAction::RealeaseResources()
{
	CloseiFlyTekTTsSocket();

	iFlyTekAudioData.Empty();

	SetReadyToDestroy();

}

void UXGiFlyTekTTSAsyncAction::CreateiFlyTekTTSSocket()
{
	FString APPID = UXGiFlyTekCoreSettings::GetXGiFlyTekCoreSettings()->AppID;
	FString APIKey = UXGiFlyTekCoreSettings::GetXGiFlyTekCoreSettings()->APIKeyTTSStream;
	FString APISecret = UXGiFlyTekCoreSettings::GetXGiFlyTekCoreSettings()->APISecretTTSStream;

	FString HttpDate = FDateTime::Now().UtcNow().ToHttpDate();

	FString ServerURL = TEXT("");

	FString host = TEXT("host: tts-api.xfyun.cn\n");
	FString date = TEXT("date: ") + HttpDate + TEXT("\n");
	FString requireLine = TEXT("GET /v2/tts HTTP/1.1");

	FString signature_origin = host + date + requireLine;

	FString	signature = UXGiFlyTekCoreBPLibrary::iFlyTekTTSHMACSHA256(APISecret, signature_origin);

	FString authorization_origin =
		FString::Printf(TEXT("api_key=\"%s\",algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"%s\""), *APIKey, *signature);

	FString authorization = FBase64::Encode(authorization_origin);

	HttpDate.ReplaceInline(TEXT(","), TEXT("%2C"));
	HttpDate.ReplaceInline(TEXT(" "), TEXT("%20"));
	HttpDate.ReplaceInline(TEXT(":"), TEXT("%3A"));

	ServerURL = TEXT("ws://tts-api.xfyun.cn/v2/tts?authorization=");
	ServerURL += authorization;
	ServerURL += TEXT("&date=");
	ServerURL += HttpDate;
	ServerURL += TEXT("&host=tts-api.xfyun.cn");

	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL);
	Socket->OnConnected().AddUObject(this, &UXGiFlyTekTTSAsyncAction::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UXGiFlyTekTTSAsyncAction::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UXGiFlyTekTTSAsyncAction::OnClosed);
	Socket->OnMessage().AddUObject(this, &UXGiFlyTekTTSAsyncAction::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UXGiFlyTekTTSAsyncAction::OnMessageSent);

	Socket->Connect();

}

void UXGiFlyTekTTSAsyncAction::CloseiFlyTekTTsSocket()
{

	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}

	Socket.Reset();


}

void UXGiFlyTekTTSAsyncAction::OnConnected()
{

	UE_LOG(LogXGiFlyTekTTS, Display, TEXT("%s"), *FString(__FUNCTION__));

	char* CharPtr = TCHAR_TO_UTF8(*iFlyTekText);
	int32 Length = strlen(CharPtr);

	FString Base64iFlyTekText = FBase64::Encode((uint8*)CharPtr, Length);

	iFlyTekTTSReqInfo.common.app_id = UXGiFlyTekCoreSettings::GetXGiFlyTekCoreSettings()->AppID;
	iFlyTekTTSReqInfo.data.text = Base64iFlyTekText;

	iFlyTekTTSReqInfo.business.speed = FMath::Clamp(iFlyTekTTSReqInfo.business.speed, 0, 100);
	iFlyTekTTSReqInfo.business.volume = FMath::Clamp(iFlyTekTTSReqInfo.business.volume, 0, 100);
	iFlyTekTTSReqInfo.business.pitch = FMath::Clamp(iFlyTekTTSReqInfo.business.pitch, 0, 100);




	FString JsonStr;
	FJsonObjectConverter::UStructToJsonObjectString(iFlyTekTTSReqInfo, JsonStr);

	Socket->Send(JsonStr);

}

void UXGiFlyTekTTSAsyncAction::OnConnectionError(const FString& ErrorMessage)
{
	UE_LOG(LogXGiFlyTekTTS, Error, TEXT("[%s]:ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *ErrorMessage);

	OnFail.Broadcast(false, ErrorMessage, nullptr);
	RealeaseResources();
}

void UXGiFlyTekTTSAsyncAction::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogXGiFlyTekTTS, Warning, TEXT("[%s]:ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));
	RealeaseResources();
}

void UXGiFlyTekTTSAsyncAction::OnMessage(const FString& Message)
{
	//UE_LOG(LogXGiFlyTekTTS, Log, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *Message);

	FXGiFlyTekTTSRespInfo iFlyTekTTSRespInfo;
	FJsonObjectConverter::JsonObjectStringToUStruct(Message, &iFlyTekTTSRespInfo);

	if (iFlyTekTTSRespInfo.code == 0)
	{
		FString Audio = iFlyTekTTSRespInfo.data.audio;
		int32	Status = iFlyTekTTSRespInfo.data.status;
		FString Ced = iFlyTekTTSRespInfo.data.ced;

		bool bRightStatus = Status == 1 || Status == 2;

		if (!Audio.IsEmpty() && bRightStatus)
		{
			TArray<uint8> AudioData;
			FBase64::Decode(Audio, AudioData);
			iFlyTekAudioData.Append(AudioData);
		}

		if (Status == 2)
		{
			

			if (bSaveToLocal)
			{
				TArray<uint8> WaveData;
				UXGiFlyTekCoreBPLibrary::ConvertPCMToWave(iFlyTekAudioData, WaveData);

				FFileHelper::SaveArrayToFile(WaveData, *SaveFileFullPath);
			}

			USoundWave* SoundWave = UXGiFlyTekCoreBPLibrary::ImportPCMToSoundWave(iFlyTekAudioData);

			UE_LOG(LogXGiFlyTekTTS, Display, TEXT("Text To SPeech Success!"));
			OnSuccess.Broadcast(true, TEXT("Text To SPeech Success!"), SoundWave);
			RealeaseResources();
		}


	}
	else
	{
		FString ErrorMessage = FString::Printf(TEXT("[%s]-MessageError-Code:[%d]-Message:[%s]-sid-[%s]"),
			*FString(__FUNCTION__),
			iFlyTekTTSRespInfo.code,
			*(iFlyTekTTSRespInfo.message),
			*(iFlyTekTTSRespInfo.sid));

		UE_LOG(LogXGiFlyTekTTS, Error, TEXT("[%s]"), *ErrorMessage);
		OnFail.Broadcast(false, ErrorMessage,nullptr);
		RealeaseResources();
	
	}



}

void UXGiFlyTekTTSAsyncAction::OnMessageSent(const FString& MessageString)
{

}
