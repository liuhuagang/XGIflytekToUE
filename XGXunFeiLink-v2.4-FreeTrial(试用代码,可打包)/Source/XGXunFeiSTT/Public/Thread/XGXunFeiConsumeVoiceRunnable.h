// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
class UXGXunFeiAudioCaptureSubsystem;
class UXGXunFeiRealTimeSTTSubsystem;
class UXGXunFeiVoiceDictationSubsystem;


enum class EXGXunFeiConsumeVoiceType
{
	None,
	RealtimeSTT,
	VoiceDictation
};

/**
 * FXGXunFeiConsumeVoiceRunnable
 * AudioConsume Thread. Process the data from AudioCaptureSystem,and then send it to iFlyTesk web service.
 */
class FXGXunFeiConsumeVoiceRunnable :public FRunnable
{
public:

	FXGXunFeiConsumeVoiceRunnable(FString InThreadName,
		EXGXunFeiConsumeVoiceType InConsumeVoiceType);

	virtual ~FXGXunFeiConsumeVoiceRunnable();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	FString ThreadName;

protected:

	EXGXunFeiConsumeVoiceType ConsumeVoiceType = EXGXunFeiConsumeVoiceType::None;

	FThreadSafeBool bIsRunning = true;


};
