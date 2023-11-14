// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
class UXGiFlyTekAudioCaptureSubsystem;
class UXGiFlyTekRealTimeSTTSubsystem;
class FXGiFlyTekConsumeVoiceRunnable :public FRunnable
{
public:
	FXGiFlyTekConsumeVoiceRunnable(FString InThreadName, UXGiFlyTekAudioCaptureSubsystem* InAudioCaptureSubsystem, UXGiFlyTekRealTimeSTTSubsystem* InRealTimeSTTSubsystem);
	virtual ~FXGiFlyTekConsumeVoiceRunnable();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	FString ThreadName;
protected:

	FThreadSafeBool bIsRunning;
	FCriticalSection CriticalSection;

	UXGiFlyTekAudioCaptureSubsystem* AudioCaptureSubsystem;
	UXGiFlyTekRealTimeSTTSubsystem* RealTimeSTTSubsystem;
};
