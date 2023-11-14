// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGKeDaXunFeiSoundBPLibrary.h"
#include "XGKeDaXunFeiSound.h"
#include "XGKeDaXunFeiSocketSubsystem.h"
#include "Kismet/GameplayStatics.h"

UXGKeDaXunFeiSoundBPLibrary::UXGKeDaXunFeiSoundBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UXGKeDaXunFeiSoundBPLibrary::XGBeginSpeachToText(const UObject* WorldContextObject, FXGBeginSpeachToTextDelegate XGBeginSpeachToTextDelegate, FXGSpeachToTextDelegate XGSpeachToTextDelegat)
{
	if (!WorldContextObject)
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UXGKeDaXunFeiSocketSubsystem* SocketSystem = GameInstance->GetSubsystem<UXGKeDaXunFeiSocketSubsystem>();

	SocketSystem->XGBeginSpeachToText(XGBeginSpeachToTextDelegate, XGSpeachToTextDelegat);

}

void UXGKeDaXunFeiSoundBPLibrary::XGStopSpeachToText(const UObject* WorldContextObject)
{

	FString ChineseTest1 = TEXT("你好");

	if (!WorldContextObject)
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UXGKeDaXunFeiSocketSubsystem* SocketSystem = GameInstance->GetSubsystem<UXGKeDaXunFeiSocketSubsystem>();
	SocketSystem->XGStopSpeachToText();

}

void UXGKeDaXunFeiSoundBPLibrary::XGBeginSpeachToTextFullConfig(const UObject* WorldContextObject, 
const FXGRealTimeSpeechToTextRequestType& InXGRealTimeSpeechToTextType, 
FXGBeginSpeachToTextDelegate XGBeginSpeachToTextDelegate, 
FXGSpeachToTextDelegate XGSpeachToTextDelegat,
FXGSpeachToTextWithoutTranslateDelegate XGSpeachToTextWithoutTranslateDelegate
)
{

	if (!WorldContextObject)
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UXGKeDaXunFeiSocketSubsystem* SocketSystem = GameInstance->GetSubsystem<UXGKeDaXunFeiSocketSubsystem>();

	SocketSystem->XGBeginSpeachToTextConfig(InXGRealTimeSpeechToTextType,
	XGBeginSpeachToTextDelegate, 
	XGSpeachToTextDelegat,
	XGSpeachToTextWithoutTranslateDelegate
	);


}

