// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include <ISubmixBufferListener.h>
#include "SceneVoice.generated.h"



UCLASS()
class ASceneVoice :public AActor, public ISubmixBufferListener
{
	GENERATED_BODY()

	public :
		ASceneVoice();

	
public:

	virtual void OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock) override;




protected:
	void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



protected:
	FAudioDeviceHandle AudioDevice;


};