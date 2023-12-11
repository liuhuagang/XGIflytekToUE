// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
class UXGXunFeiAudioCaptureSubsystem;
class UXGXunFeiRealTimeSTTSubsystem;

/**
 * FXGXunFeiConsumeVoiceRunnable
 * AudioConsume Thread. Process the data from AudioCaptureSystem,and then send it to iFlyTesk web service.
 */
class FXGXunFeiConsumeVoiceRunnable :public FRunnable
{
public:
	FXGXunFeiConsumeVoiceRunnable(FString InThreadName,
		TWeakObjectPtr<UXGXunFeiRealTimeSTTSubsystem> InRealTimeSTTSubsystem);
	virtual ~FXGXunFeiConsumeVoiceRunnable();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	FString ThreadName;
protected:



	FThreadSafeBool bIsRunning;
	
	FCriticalSection CriticalSection;

	TWeakObjectPtr<UXGXunFeiRealTimeSTTSubsystem> RealTimeSTTSubsystem;
};
