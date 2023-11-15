// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiTTSAsyncAction.h"

#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "JsonObjectConverter.h"

#include "Misc/Paths.h"
#include "Misc/Base64.h"
#include "Misc/FileHelper.h"
#include "Containers/StringConv.h"
#include "XGXunFeiCoreSettings.h"
#include "XGXunFeiCoreBPLibrary.h"
#include "LogXGXunFeiTTS.h"
#include "XGXunFeiTTSRespType.h"


UXGXunFeiTTSAsyncAction::UXGXunFeiTTSAsyncAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}


UXGXunFeiTTSAsyncAction* UXGXunFeiTTSAsyncAction::XGXunFeiTextToSpeech(
	UObject* WorldContextObject,
	const FString& InText,
	bool bInSaveToLocal,
	const FString& InSaveFileFullPath,
	FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo)
{
	UXGXunFeiTTSAsyncAction* XunFeiTTSAsyncAction = NewObject<UXGXunFeiTTSAsyncAction>();

	XunFeiTTSAsyncAction->SetXunFeiText(InText);
	XunFeiTTSAsyncAction->SetIsSaveToLocal(bInSaveToLocal);
	XunFeiTTSAsyncAction->SetSaveFileFullPath(InSaveFileFullPath);
	XunFeiTTSAsyncAction->SetXunFeiTTSReqInfo(InXunFeiTTSReqInfo);



	XunFeiTTSAsyncAction->RegisterWithGameInstance(WorldContextObject);


	return XunFeiTTSAsyncAction;
}
void UXGXunFeiTTSAsyncAction::Activate()
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

			UE_LOG(LogXGXunFeiTTS, Error, TEXT("%s"), *Message);
			OnFail.Broadcast(false, Message, nullptr);
			RealeaseResources();
		}

		if (!bExtension)
		{
			FString Message = FString::Printf(TEXT("[%s]:FileName is Wrong,it must endwith [.wav] ! FilePath: [%s]"), *FString(__FUNCTION__), *SaveFileFullPath);
			UE_LOG(LogXGXunFeiTTS, Error, TEXT("%s"), *Message);
			OnFail.Broadcast(false, Message, nullptr);
			RealeaseResources();
		}

	}

	if (XunFeiText.IsEmpty())
	{
		FString Message = FString::Printf(TEXT("[%s]:InText is Empty,it must be input!"), *FString(__FUNCTION__));
		UE_LOG(LogXGXunFeiTTS, Error, TEXT("%s"), *Message);
		OnFail.Broadcast(false, Message, nullptr);
		RealeaseResources();
	}

	CreateXunFeiTTSSocket();

}

void UXGXunFeiTTSAsyncAction::RealeaseResources()
{
	CloseXunFeiTTsSocket();

	XunFeiAudioData.Empty();

	SetReadyToDestroy();

}

void UXGXunFeiTTSAsyncAction::CreateXunFeiTTSSocket()
{
	FString APPID = UXGXunFeiCoreSettings::GetXGXunFeiCoreSettings()->AppID;
	FString APIKey = UXGXunFeiCoreSettings::GetXGXunFeiCoreSettings()->APIKeyTTSStream;
	FString APISecret = UXGXunFeiCoreSettings::GetXGXunFeiCoreSettings()->APISecretTTSStream;

	FString HttpDate = FDateTime::Now().UtcNow().ToHttpDate();

	FString ServerURL = TEXT("");

	FString host = TEXT("host: tts-api.xfyun.cn\n");
	FString date = TEXT("date: ") + HttpDate + TEXT("\n");
	FString requireLine = TEXT("GET /v2/tts HTTP/1.1");

	FString signature_origin = host + date + requireLine;

	FString	signature = UXGXunFeiCoreBPLibrary::XunFeiTTSHMACSHA256(APISecret, signature_origin);

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
	Socket->OnConnected().AddUObject(this, &UXGXunFeiTTSAsyncAction::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UXGXunFeiTTSAsyncAction::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UXGXunFeiTTSAsyncAction::OnClosed);
	Socket->OnMessage().AddUObject(this, &UXGXunFeiTTSAsyncAction::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UXGXunFeiTTSAsyncAction::OnMessageSent);

	Socket->Connect();

}

void UXGXunFeiTTSAsyncAction::CloseXunFeiTTsSocket()
{

	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}

	Socket.Reset();


}

void UXGXunFeiTTSAsyncAction::OnConnected()
{

	UE_LOG(LogXGXunFeiTTS, Display, TEXT("%s"), *FString(__FUNCTION__));


	UE_LOG(LogTemp, Error, TEXT("Src:[%s]"), *XunFeiText);

	auto UTF8String= StringCast<UTF8CHAR>(*XunFeiText);

	FString Base64XunFeiText = FBase64::Encode((uint8*)UTF8String.Get(), UTF8String.Length());

	TArray<uint8> BackUTF8;
    FBase64::Decode(Base64XunFeiText, BackUTF8);
	
	//check right str after encode~
	//FString Reulust = StringCast<TCHAR>((UTF8CHAR*)BackUTF8.GetData(), BackUTF8.Num()).Get();
	//UE_LOG(LogTemp,Error,TEXT("[%s]"), *Reulust);


	XunFeiTTSReqInfo.common.app_id = UXGXunFeiCoreSettings::GetXGXunFeiCoreSettings()->AppID;
	XunFeiTTSReqInfo.data.text = Base64XunFeiText;

	XunFeiTTSReqInfo.business.speed = FMath::Clamp(XunFeiTTSReqInfo.business.speed, 0, 100);
	XunFeiTTSReqInfo.business.volume = FMath::Clamp(XunFeiTTSReqInfo.business.volume, 0, 100);
	XunFeiTTSReqInfo.business.pitch = FMath::Clamp(XunFeiTTSReqInfo.business.pitch, 0, 100);




	FString JsonStr;
	FJsonObjectConverter::UStructToJsonObjectString(XunFeiTTSReqInfo, JsonStr);

	Socket->Send(JsonStr);

}

void UXGXunFeiTTSAsyncAction::OnConnectionError(const FString& ErrorMessage)
{
	UE_LOG(LogXGXunFeiTTS, Error, TEXT("[%s]:ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *ErrorMessage);

	OnFail.Broadcast(false, ErrorMessage, nullptr);
	RealeaseResources();
}

void UXGXunFeiTTSAsyncAction::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogXGXunFeiTTS, Warning, TEXT("[%s]:ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));
	RealeaseResources();
}

void UXGXunFeiTTSAsyncAction::OnMessage(const FString& Message)
{
	//UE_LOG(LogXGXunFeiTTS, Log, TEXT("[%s], Message:%s"), *FString(__FUNCTION__), *Message);

	FXGXunFeiTTSRespInfo XunFeiTTSRespInfo;
	FJsonObjectConverter::JsonObjectStringToUStruct(Message, &XunFeiTTSRespInfo);

	if (XunFeiTTSRespInfo.code == 0)
	{
		FString Audio = XunFeiTTSRespInfo.data.audio;
		int32	Status = XunFeiTTSRespInfo.data.status;
		FString Ced = XunFeiTTSRespInfo.data.ced;

		bool bRightStatus = Status == 1 || Status == 2;

		if (!Audio.IsEmpty() && bRightStatus)
		{
			TArray<uint8> AudioData;
			FBase64::Decode(Audio, AudioData);
			XunFeiAudioData.Append(AudioData);
		}

		if (Status == 2)
		{
			

			if (bSaveToLocal)
			{
				TArray<uint8> WaveData;
				UXGXunFeiCoreBPLibrary::ConvertPCMToWave(XunFeiAudioData, WaveData);

				FFileHelper::SaveArrayToFile(WaveData, *SaveFileFullPath);
			}

			USoundWave* SoundWave = UXGXunFeiCoreBPLibrary::ImportPCMToSoundWave(XunFeiAudioData);

			UE_LOG(LogXGXunFeiTTS, Display, TEXT("Text To SPeech Success!"));
			OnSuccess.Broadcast(true, TEXT("Text To SPeech Success!"), SoundWave);
			RealeaseResources();
		}


	}
	else
	{
		FString ErrorMessage = FString::Printf(TEXT("[%s]-MessageError-Code:[%d]-Message:[%s]-sid-[%s]"),
			*FString(__FUNCTION__),
			XunFeiTTSRespInfo.code,
			*(XunFeiTTSRespInfo.message),
			*(XunFeiTTSRespInfo.sid));

		UE_LOG(LogXGXunFeiTTS, Error, TEXT("[%s]"), *ErrorMessage);
		OnFail.Broadcast(false, ErrorMessage,nullptr);
		RealeaseResources();
	
	}



}

void UXGXunFeiTTSAsyncAction::OnMessageSent(const FString& MessageString)
{

}
