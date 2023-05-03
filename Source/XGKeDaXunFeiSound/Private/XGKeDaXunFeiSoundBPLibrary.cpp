// Copyright Epic Games, Inc. All Rights Reserved.

#include "XGKeDaXunFeiSoundBPLibrary.h"
#include "XGKeDaXunFeiSound.h"
#include "Core/XGKeDaXunFeiSocketSubsystem.h"
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
	if (!WorldContextObject)
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UXGKeDaXunFeiSocketSubsystem* SocketSystem = GameInstance->GetSubsystem<UXGKeDaXunFeiSocketSubsystem>();
	SocketSystem->XGStopSpeachToText();

}

