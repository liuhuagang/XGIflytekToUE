// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiTTSAsyncAction.h"

#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "JsonObjectConverter.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/Paths.h"
#include "Misc/Base64.h"
#include "Misc/FileHelper.h"
#include "Containers/StringConv.h"
#include "Sound/SoundWave.h"


#include "XGXunFeiCoreSettings.h"
#include "XGXunFeiCoreBPLibrary.h"
#include "LogXGXunFeiTTS.h"
#include "XGXunFeiTTSRespType.h"


UXGXunFeiTTSAsyncAction::UXGXunFeiTTSAsyncAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}

UXGXunFeiTTSAsyncAction::~UXGXunFeiTTSAsyncAction()
{
	RealeaseResources();
}


UXGXunFeiTTSAsyncAction* UXGXunFeiTTSAsyncAction::XGXunFeiTextToSpeech(
	UObject* WorldContextObject,
	FString InTTSAppID,
	FString InTTSAPISecret,
	FString InTTSAPIKey,
	const FString& InText,
	bool bInSaveToLocal,
	const FString& InSaveFileFullPath,
	FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo)
{
	UXGXunFeiTTSAsyncAction* XunFeiTTSAsyncAction = NewObject<UXGXunFeiTTSAsyncAction>();


	XunFeiTTSAsyncAction->TTSAppID = InTTSAppID;
	XunFeiTTSAsyncAction->TTSAPISecret = InTTSAPISecret;
	XunFeiTTSAsyncAction->TTSAPIKey = InTTSAPIKey;


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

	Then.Broadcast(AsyncID, false, TEXT("TTS is just started.Please wait to finish ! "), nullptr);

}

void UXGXunFeiTTSAsyncAction::Activate_Internal()
{

	if (bSaveToLocal)
	{
		FString FileDirectory = FPaths::GetPath(SaveFileFullPath);
		bool bDirectory = FPaths::DirectoryExists(FileDirectory);
		FString FileExtension = FPaths::GetExtension(SaveFileFullPath);
		bool bExtension = FileExtension.Equals(TEXT("wav"));

		if (!bDirectory)
		{
			FString DirectoryErrorMessage = FString::Printf(TEXT("FileDirectory don't exist ! FilePath: [%s]"), *SaveFileFullPath);

			UE_LOG(LogXGXunFeiTTS, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *DirectoryErrorMessage);

			CallOnFail(false, DirectoryErrorMessage);

			RealeaseResources();

			return;
		}

		if (!bExtension)
		{
			FString FileNameErroeMessage = FString::Printf(TEXT("FileName is Wrong,it must endwith [.wav] ! FilePath: [%s]"), *FString(__FUNCTION__), *SaveFileFullPath);

			UE_LOG(LogXGXunFeiTTS, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *FileNameErroeMessage);

			CallOnFail(false, FileNameErroeMessage);

			RealeaseResources();

			return;
		}

	}

	if (XunFeiText.IsEmpty())
	{
		FString TextErrorMessage = FString::Printf(TEXT("InText is Empty,it must be input!"));

		UE_LOG(LogXGXunFeiTTS, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *TextErrorMessage);

		CallOnFail(false, TextErrorMessage);

		RealeaseResources();

		return;
	}

	AsyncTask(ENamedThreads::GameThread, [this]() {

		CreateXunFeiTTSSocket();
		});


}

void UXGXunFeiTTSAsyncAction::RealeaseResources()
{
	OnSoundWaveSuccess.Clear();

	OnWavFileSuccess.Clear();

	OnSoundWaveFail.Clear();

	CloseXunFeiTTsSocket();

	XunFeiAudioData.Empty();

	SetReadyToDestroy();

}

void UXGXunFeiTTSAsyncAction::CreateXunFeiTTSSocket()
{
	FString APPID = TTSAppID;
	FString APISecret = TTSAPISecret;
	FString APIKey = TTSAPIKey;


	FString HttpDate = FDateTime::Now().UtcNow().ToHttpDate();

	FString ServerURL = TEXT("");

	FString host = TEXT("host: tts-api.xfyun.cn\n");
	FString date = TEXT("date: ") + HttpDate + TEXT("\n");
	FString requireLine = TEXT("GET /v2/tts HTTP/1.1");

	FString signature_origin = host + date + requireLine;

	FString	signature = UXGXunFeiCoreBPLibrary::XunFeiTTSHMACSHA256(APISecret, signature_origin);

	FString authorization_origin =
		FString::Printf(TEXT("api_key=\"%s\", algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"%s\""), *APIKey, *signature);

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
	UE_LOG(LogXGXunFeiTTS, Display, TEXT("[%s]:[%s] Connect Success!"), *FString(__FUNCTION__), *(AsyncID.ToString()));


	//UE_LOG(LogTemp, Error, TEXT("Src:[%s]"), *XunFeiText);

	auto UTF8String = StringCast<UTF8CHAR>(*XunFeiText);
	FString Base64XunFeiText = FBase64::Encode((uint8*)UTF8String.Get(), UTF8String.Length());

	//	//check right str after encode~
	//TArray<uint8> BackUTF8;
	//FBase64::Decode(Base64XunFeiText, BackUTF8);
	//FString Reulust = StringCast<TCHAR>((UTF8CHAR*)BackUTF8.GetData(), BackUTF8.Num()).Get();
	//UE_LOG(LogTemp,Error,TEXT("[%s]"), *Reulust);


	XunFeiTTSReqInfo.common.app_id = TTSAppID;
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

	UE_LOG(LogXGXunFeiTTS, Error, TEXT("[%s]:[%s] ConnectError,Message:[%s]!"), *FString(__FUNCTION__), *(AsyncID.ToString()), *ErrorMessage);

	CallOnFail(false, ErrorMessage);
	RealeaseResources();

}

void UXGXunFeiTTSAsyncAction::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{

	UE_LOG(LogXGXunFeiTTS, Warning, TEXT("[%s]:[%s] ConnectOnClosed,StatusCode:[%d],Reason:[%s],bWasClean:[%s]!"),
		*FString(__FUNCTION__), *(AsyncID.ToString()), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));


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

				CallGenerateWavFile();

			}

			USoundWave* SoundWave = UXGXunFeiCoreBPLibrary::ImportPCMToSoundWave(XunFeiAudioData);

			UE_LOG(LogXGXunFeiTTS, Log, TEXT("[%s]:Text To SPeech Success!"), *FString(__FUNCTION__));

			CallOnSoundWaveSuccess(true, TEXT("Text To SPeech Success!"), SoundWave);

			RealeaseResources();

		}


	}
	else
	{
		FString ErrorMessage = FString::Printf(TEXT("MessageError-Code:[%d]-Message:[%s]-sid-[%s]"),
			XunFeiTTSRespInfo.code,
			*(XunFeiTTSRespInfo.message),
			*(XunFeiTTSRespInfo.sid));

		UE_LOG(LogXGXunFeiTTS, Error, TEXT("[%s]:[%s]"), *FString(__FUNCTION__), *ErrorMessage);

		CallOnFail(false, ErrorMessage);

		RealeaseResources();

	}



}

void UXGXunFeiTTSAsyncAction::OnMessageSent(const FString& MessageString)
{


}

void UXGXunFeiTTSAsyncAction::CallOnSoundWaveSuccess(bool InbResult, FString InMessage, USoundWave* InSoundWavePtr)
{
	FXGXunFeiTTSDelegate TempXunFeiTTSDelegate = OnSoundWaveSuccess;
	FGuid InAsyncID = AsyncID;

	InSoundWavePtr->AddToRoot();

	AsyncTask(ENamedThreads::GameThread, [=]() {

		InSoundWavePtr->RemoveFromRoot();

		TempXunFeiTTSDelegate.Broadcast(InAsyncID, InbResult, InMessage, InSoundWavePtr);


		});
}



void UXGXunFeiTTSAsyncAction::CallOnFail(bool InbResult, FString InMessage, USoundWave* InSoundWavePtr)
{
	FXGXunFeiTTSDelegate TempXunFeiTTSDelegate = OnSoundWaveFail;
	FGuid InAsyncID = AsyncID;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempXunFeiTTSDelegate.Broadcast(InAsyncID, InbResult, InMessage, InSoundWavePtr);


		});

}

void UXGXunFeiTTSAsyncAction::CallGenerateWavFile()
{
	FXGXunFeiTTSDelegate TempOnWavFileSuccess = OnWavFileSuccess;
	FXGXunFeiTTSDelegate TempOnWavFileFail = OnWavFileFail;


	TArray<uint8> AudioData = XunFeiAudioData;
	FString WavFilePath = SaveFileFullPath;
	FGuid OutAsyncID = AsyncID;

	AsyncTask(ENamedThreads::AnyThread, [TempOnWavFileSuccess, TempOnWavFileFail, AudioData, WavFilePath, OutAsyncID]() {


		TArray<uint8> WaveData;
		UXGXunFeiCoreBPLibrary::ConvertPCMToWave(AudioData, WaveData);

		bool bSave = FFileHelper::SaveArrayToFile(WaveData, *WavFilePath);
		FXGXunFeiTTSDelegate TempXunFeiTTSDelegate;
		FString WavFileMessage = TEXT("");

		if (bSave)
		{
			TempXunFeiTTSDelegate = TempOnWavFileSuccess;
			WavFileMessage = TEXT("The wav file has been finished !");
		}
		else
		{
			TempXunFeiTTSDelegate = TempOnWavFileFail;
			WavFileMessage = TEXT("The wav file failed to be generated");
		}

		AsyncTask(ENamedThreads::GameThread, [TempXunFeiTTSDelegate, bSave, WavFileMessage, OutAsyncID]() {

			TempXunFeiTTSDelegate.Broadcast(OutAsyncID, bSave, WavFileMessage, nullptr);

			});



		});





}
