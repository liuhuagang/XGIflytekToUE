// Copyright Xiao Gang. All Rights Reserved.

#include "XGiFlyTekToUEBPLibrary.h"
#include "XGiFlyTekRealTimeSTTSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"


void UXGiFlyTekToUEBPLibrary::XGiFlyTekBeginRealTimeSpeechToText(const UObject* WorldContextObject, FXGiFlyTekRealTimeSTTReqInfo InRealTimeSTTReqInfo, FXGiFlyTekInitRealTimeSTTDelegate InInitRealTimeSTTDelegate, FXGiFlyTekRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate, FXGiFlyTekRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGiFlyTekRealTimeSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGiFlyTekRealTimeSTTSubsystem>();
		RealTimeSTTSubsystem->XGBeginRealTimeSpeechToText(InRealTimeSTTReqInfo, InInitRealTimeSTTDelegate, InRealTimeSTTNoTranslateDelegate, InRealTimeSTTTranslateDelegate);

	}
	else
	{
		InInitRealTimeSTTDelegate.ExecuteIfBound(false, TEXT("WorldContextObject is null"));
	}
}

void UXGiFlyTekToUEBPLibrary::XGiFlyTekStopRealTimeSpeechToText(const UObject* WorldContextObject)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGiFlyTekRealTimeSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGiFlyTekRealTimeSTTSubsystem>();
		RealTimeSTTSubsystem->XGStopRealTimeSpeechToText();

	}
	else
	{

	}
}

void UXGiFlyTekToUEBPLibrary::XGiFlyTekTextToSpeech(UObject* WorldContextObject, const FString& InText, bool bInSaveToLocal, const FString& InSaveFileFullPath, FXGiFlyTekTTSReqInfo IniFlyTekTTSReqInfo, FXGiFlyTekTTSDelegate OniFlyTekTTSSuccess, FXGiFlyTekTTSDelegate OniFlyTekTTSFail)
{
	if (!WorldContextObject||!WorldContextObject->GetWorld())
	{
		OniFlyTekTTSFail.Broadcast(false,TEXT("WorldContextObject is null"),nullptr);
		return ;
	}

	UXGiFlyTekTTSAsyncAction* XGiFlyTekTTSAsyncAction = UXGiFlyTekTTSAsyncAction::XGiFlyTekTextToSpeech(WorldContextObject, InText, bInSaveToLocal, InSaveFileFullPath, IniFlyTekTTSReqInfo);
	
	XGiFlyTekTTSAsyncAction->OnSuccess = OniFlyTekTTSSuccess;
	XGiFlyTekTTSAsyncAction->OnFail = OniFlyTekTTSFail;

	XGiFlyTekTTSAsyncAction->Activate();


}
