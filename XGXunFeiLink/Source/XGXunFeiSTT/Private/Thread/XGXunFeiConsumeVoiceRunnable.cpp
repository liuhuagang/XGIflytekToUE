// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiConsumeVoiceRunnable.h"
#include "XGXunFeiAudioCaptureSubsystem.h"
#include "XGXunFeiRealTimeSTTSubsystem.h"


FXGXunFeiConsumeVoiceRunnable::FXGXunFeiConsumeVoiceRunnable(FString InThreadName,
	UXGXunFeiAudioCaptureSubsystem* InAudioCaptureSubsystem,
	UXGXunFeiRealTimeSTTSubsystem* InRealTimeSTTSubsystem)
:ThreadName(InThreadName)
, AudioCaptureSubsystem(InAudioCaptureSubsystem)
, RealTimeSTTSubsystem(InRealTimeSTTSubsystem)
{


}

FXGXunFeiConsumeVoiceRunnable::~FXGXunFeiConsumeVoiceRunnable()
{

}

bool FXGXunFeiConsumeVoiceRunnable::Init()
{
	return true;
}

uint32 FXGXunFeiConsumeVoiceRunnable::Run()
{
	bIsRunning =true;

	while (bIsRunning)
	{
		FPlatformProcess::Sleep(0.04);

		TArray<float> VoiceData ;
		bool bRightVoice = AudioCaptureSubsystem->GetVoiceData(VoiceData);

		if (bRightVoice)
		{
			RealTimeSTTSubsystem->SendVoiceData(VoiceData);
		}
	}
	return 0;
}

void FXGXunFeiConsumeVoiceRunnable::Stop()
{
	bIsRunning = false;

}

void FXGXunFeiConsumeVoiceRunnable::Exit()
{
	AudioCaptureSubsystem->ClearVoiceData();
}
