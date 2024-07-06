// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioCapture.h"
#include "XGXunFeiAudioCaptureSubsystem.generated.h"


enum class EXGXunFeiAudioCaptureType
{
	None,
	RealtimeSTT,
	VoiceDictation_8k,
	VoiceDictation_16k,
};

/**
 *	UXGXunFeiAudioCaptureSubsystem
 *	Audio capture from deivce.
 *	if	you have no audio input device,It will not work.
 */
 struct FAudioGeneratorHandle;
UCLASS()
class XGXUNFEISTT_API UXGXunFeiAudioCaptureSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class FXGXunFeiConsumeVoiceRunnable;
	friend class UXGXunFeiRealTimeSTTSubsystem;
	friend class UXGXunFeiVoiceDictationSubsystem;
	friend class UXGXunFeiLinkBPLibrary;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

protected:
	
	static UXGXunFeiAudioCaptureSubsystem* Get();

	void StartCapturingAudio(EXGXunFeiAudioCaptureType InAuidoCaptureType= EXGXunFeiAudioCaptureType::RealtimeSTT);

	void StopCapturingAudio();

	bool IsCapturingAudio();

	bool GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo);

	float GetVolumeSize();

	void  SetVolumeSize(float InVolumeSize);

	static void AppendVoiceData(const float* InAudio, int32 NumSamples);

	static bool GetVoiceData(TArray<float>& OutVoiceData);

	static void ClearVoiceData();

	static void OnAudioGenerate(const float* InAudio, int32 NumSamples);

	static TArray<float> LinearResample(TArray<float>& InData,int32 OutDataSize);

protected:

	static FAudioCaptureDeviceInfo AudioCaptureDeviceInfo;

	static UXGXunFeiAudioCaptureSubsystem* XunFeiAudioCaptureSubsystemPtr;

	static int32 AudioCaptureSampleRate;

	static int32 OnceSendFloatDataNum;

	static int32 AudioCaptureCoefficient;

	float VolumeSize =0.f;

	UPROPERTY()
	UAudioCapture* XGAudioCapture;

	FAudioGeneratorHandle AudioGeneratorHandle;

	static FCriticalSection VoiceDataSection;

	static TArray<float> NewAllVoiceData;


};