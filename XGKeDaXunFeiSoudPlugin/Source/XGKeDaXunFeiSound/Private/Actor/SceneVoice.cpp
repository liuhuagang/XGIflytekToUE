// Copyright 2023 Xiao Gang. All Rights Reserved.
#include "SceneVoice.h"
#include <AudioDeviceManager.h>
#include <AudioDeviceHandle.h>
#include <AudioDevice.h>
ASceneVoice::ASceneVoice()
{

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

}

void ASceneVoice::OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]-NumSamples :{%d}-NumChannels-{%d} SampleRate -{%d} AudioClock -{%f}"), *FString(__FUNCTION__), NumSamples, NumChannels, SampleRate, AudioClock);

}

void ASceneVoice::BeginPlay()
{
	Super::BeginPlay();
	AudioDevice = GetWorld()->GetAudioDevice();


	if (AudioDevice)
	{
		AudioDevice->RegisterSubmixBufferListener(this);
	}



}

void ASceneVoice::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AudioDevice)
	{
		AudioDevice->UnregisterSubmixBufferListener(this);
	}




	Super::EndPlay(EndPlayReason);
}
