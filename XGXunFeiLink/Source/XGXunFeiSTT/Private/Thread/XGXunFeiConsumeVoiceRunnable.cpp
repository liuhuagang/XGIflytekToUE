// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiConsumeVoiceRunnable.h"
#include "XGXunFeiAudioCaptureSubsystem.h"
#include "XGXunFeiRealTimeSTTSubsystem.h"


FXGXunFeiConsumeVoiceRunnable::FXGXunFeiConsumeVoiceRunnable(FString InThreadName,
	TWeakObjectPtr<UXGXunFeiRealTimeSTTSubsystem> InRealTimeSTTSubsystem)
	:ThreadName(InThreadName)
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
	bIsRunning = true;

	while (true)
	{

		FPlatformProcess::Sleep(0.04);

		TArray<float> VoiceData;


		bool bRightVoice = UXGXunFeiAudioCaptureSubsystem::GetVoiceData(VoiceData);


		FScopeLock ConsumeRunnableLcoak(&CriticalSection);

		if (!bIsRunning)
		{
			break;
		}

		if (bRightVoice)
		{
			RealTimeSTTSubsystem->SendVoiceData(VoiceData);
		}


	}
	return 0;
}

void FXGXunFeiConsumeVoiceRunnable::Stop()
{
	FScopeLock ConsumeRunnableLcoak(&CriticalSection);

	bIsRunning = false;

}

void FXGXunFeiConsumeVoiceRunnable::Exit()
{
	UXGXunFeiAudioCaptureSubsystem::ClearVoiceData();
}
