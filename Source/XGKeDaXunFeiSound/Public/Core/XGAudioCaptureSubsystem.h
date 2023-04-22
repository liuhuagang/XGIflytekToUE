
#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "AudioCapture.h"
#include "XGAudioCaptureSubsystem.generated.h"

/**
 *	可以正常使用但是在没有声频输入时会报错
 */
UCLASS()
class XGKEDAXUNFEISOUND_API UXGAudioCaptureSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	virtual void    Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool    IsTickable() const override;

public:
	// Starts capturing audio
	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
		void StartCapturingAudio();

	// Stops capturing audio
	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
		void StopCapturingAudio();

	// Returns true if capturing audio
	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
		bool IsCapturingAudio();

	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
		bool GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo);

	// TODO 垃圾代码等待优化
	//一帧是1024个float
	static	float* XGAudio;
	static	int32 XGNumSamples;
	static TArray<float*> InAudios;


protected:

	static	void OnAudioGenerate(const float* InAudio, int32 NumSamples);

	static int32 IndexSend;
	static float* ToSendData;

public:
	static	FCriticalSection XGAudioCriticalSection;
	static	TArray<float> AudioData;



protected:
	UPROPERTY()
	UAudioCapture* XGAudioCapture;

	
};