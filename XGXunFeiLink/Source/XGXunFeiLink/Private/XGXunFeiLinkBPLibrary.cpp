// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiLinkBPLibrary.h"

#include "XGXunFeiRealTimeSTTSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"



void UXGXunFeiLinkBPLibrary::XGXunFeiBeginRealTimeSpeechToText(const UObject* WorldContextObject, FXGXunFeiRealTimeSTTReqInfo InRealTimeSTTReqInfo, FXGXunFeiInitRealTimeSTTDelegate InInitRealTimeSTTDelegate, FXGXunFeiRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate, FXGXunFeiRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate)
{
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
		UXGXunFeiRealTimeSTTSubsystem* RealTimeSTTSubsystem = GameInstance->GetSubsystem<UXGXunFeiRealTimeSTTSubsystem>();
		RealTimeSTTSubsystem->XGBeginRealTimeSpeechToText(InRealTimeSTTReqInfo, InInitRealTimeSTTDelegate, InRealTimeSTTNoTranslateDelegate, InRealTimeSTTTranslateDelegate);

	}
	else
	{
		InInitRealTimeSTTDelegate.ExecuteIfBound(false, TEXT("WorldContextObject is null"));
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

void UXGXunFeiLinkBPLibrary::XGXunFeiTextToSpeech(UObject* WorldContextObject, const FString& InText, bool bInSaveToLocal, const FString& InSaveFileFullPath, FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo, FXGXunFeiTTSDelegate OnXunFeiTTSSuccess, FXGXunFeiTTSDelegate OnXunFeiTTSFail)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		OnXunFeiTTSFail.Broadcast(false, TEXT("WorldContextObject is null"), nullptr);
		return;
	}

	UXGXunFeiTTSAsyncAction* XGXunFeiTTSAsyncAction = UXGXunFeiTTSAsyncAction::XGXunFeiTextToSpeech(WorldContextObject, InText, bInSaveToLocal, InSaveFileFullPath, InXunFeiTTSReqInfo);

	XGXunFeiTTSAsyncAction->OnSuccess = OnXunFeiTTSSuccess;
	XGXunFeiTTSAsyncAction->OnFail = OnXunFeiTTSFail;

	XGXunFeiTTSAsyncAction->Activate();


}


