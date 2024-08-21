// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiLinkBPLibrary.h"

#include "XGXunFeiRealTimeSTTSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Async/Async.h"
#include "XGXunFeiCoreBPLibrary.h"


void UXGXunFeiLinkBPLibrary::XGXunFeiBeginRealTimeSpeechToText(const UObject* WorldContextObject,
	FString InSTTAppID,
	FString InSTTAPIKey,
	FXGXunFeiRealTimeSTTReqInfo InRealTimeSTTReqInfo,
	FXGXunFeiInitRealTimeSTTDelegate InRealTimeSTTInitDelegate,
	FXGXunFeiRealTimeSTTRespDelegate InRealTimeSTTRespDelegate,
	FXGXunFeiRealTimeSTTCloseDelegate InRealTimeSTTCloseDelegate,
	FGuid& OutSTTAsyncID)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGXunFeiRealTimeSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGXunFeiRealTimeSTTSubsystem>();

		OutSTTAsyncID = RealTimeSTTSubsystem->XGBeginRealTimeSpeechToText(
			InSTTAppID,
			InSTTAPIKey,
			InRealTimeSTTReqInfo,
			InRealTimeSTTInitDelegate,
			InRealTimeSTTRespDelegate,
			InRealTimeSTTCloseDelegate);

	}
	else
	{
		OutSTTAsyncID = FGuid::NewGuid();

		AsyncTask(ENamedThreads::GameThread, [InRealTimeSTTInitDelegate, OutSTTAsyncID]() {

			InRealTimeSTTInitDelegate.ExecuteIfBound(OutSTTAsyncID, false, TEXT("WorldContextObject is null"), FXGXunFeiRealTimeSTTInitInformation());

			});



	}


}

void UXGXunFeiLinkBPLibrary::XGXunFeiStopRealTimeSpeechToText(const UObject* WorldContextObject)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGXunFeiRealTimeSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGXunFeiRealTimeSTTSubsystem>();
		RealTimeSTTSubsystem->XGStopRealTimeSpeechToText();

	}
	else
	{

	}

}

FGuid UXGXunFeiLinkBPLibrary::XGXunFeiTextToSpeech(
	UObject* WorldContextObject,
	FString InTTSAppID,
	FString InTTSAPISecret,
	FString InTTSAPIKey,
	const FString& InText,
	bool bInSaveToLocal,
	const FString& InSaveFileFullPath,
	FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo,
	FXGXunFeiTTSDelegate OnXunFeiTTSSoundWaveSuccess,
	FXGXunFeiTTSDelegate OnXunFeiTTSSoundWaveFail,
	FXGXunFeiTTSDelegate OnXunFeiTTSWavFileSuccess,
	FXGXunFeiTTSDelegate OnXunFeiTTSWavFileFail)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		OnXunFeiTTSSoundWaveFail.Broadcast(FGuid(), false, TEXT("WorldContextObject is null"), nullptr);
		return FGuid();
	}

	UXGXunFeiTTSAsyncAction* XGXunFeiTTSAsyncAction = UXGXunFeiTTSAsyncAction::XGXunFeiTextToSpeech(WorldContextObject,
		InTTSAppID, InTTSAPISecret, InTTSAPIKey, InText, bInSaveToLocal, InSaveFileFullPath, InXunFeiTTSReqInfo);

	XGXunFeiTTSAsyncAction->OnSoundWaveSuccess = OnXunFeiTTSSoundWaveSuccess;
	XGXunFeiTTSAsyncAction->OnSoundWaveFail = OnXunFeiTTSSoundWaveFail;
	XGXunFeiTTSAsyncAction->OnWavFileSuccess = OnXunFeiTTSWavFileSuccess;
	XGXunFeiTTSAsyncAction->OnWavFileFail = OnXunFeiTTSWavFileFail;


	XGXunFeiTTSAsyncAction->Activate();

	return XGXunFeiTTSAsyncAction->AsyncID;


}
