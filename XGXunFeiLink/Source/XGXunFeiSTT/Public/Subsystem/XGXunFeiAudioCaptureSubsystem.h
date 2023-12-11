// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioCapture.h"
#include "XGXunFeiAudioCaptureSubsystem.generated.h"

/**
 *	UXGXunFeiAudioCaptureSubsystem
 *	Audio capture from deivce.
 *	if	you have no audio input device,It will not work.
 */
 struct FAudioGeneratorHandle;
UCLASS()
class UXGXunFeiAudioCaptureSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class FXGXunFeiConsumeVoiceRunnable;
	friend class UXGXunFeiRealTimeSTTSubsystem;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

protected:
	
	static UXGXunFeiAudioCaptureSubsystem* Get();

	void StartCapturingAudio();

	void StopCapturingAudio();

	bool IsCapturingAudio();

	bool GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo);



	static void AppendVoiceData(const TArray<float>& InVoiceData);

	static bool GetVoiceData(TArray<float>& OutVoiceData);

	static void ClearVoiceData();

	static void OnAudioGenerate(const float* InAudio, int32 NumSamples);


protected:

	static UXGXunFeiAudioCaptureSubsystem* XunFeiAudioCaptureSubsystemPtr;

	UPROPERTY()
	UAudioCapture* XGAudioCapture;

	FAudioGeneratorHandle AudioGeneratorHandle;

	static FCriticalSection VoiceDataSection;

	static TArray<float> VoiceData;


};