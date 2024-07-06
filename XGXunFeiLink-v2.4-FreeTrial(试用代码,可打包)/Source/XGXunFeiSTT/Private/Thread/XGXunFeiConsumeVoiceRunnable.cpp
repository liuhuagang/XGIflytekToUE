// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiConsumeVoiceRunnable.h"
#include "XGXunFeiAudioCaptureSubsystem.h"
#include "XGXunFeiRealTimeSTTSubsystem.h"



FXGXunFeiConsumeVoiceRunnable::FXGXunFeiConsumeVoiceRunnable(FString InThreadName,
	EXGXunFeiConsumeVoiceType InConsumeVoiceType)
	:ThreadName(InThreadName)
	, ConsumeVoiceType(InConsumeVoiceType)
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

	while (bIsRunning)
	{
		FPlatformProcess::Sleep(0.01);

		TArray<float> VoiceData;

		bool bRightVoice = UXGXunFeiAudioCaptureSubsystem::GetVoiceData(VoiceData);

		if (bRightVoice)
		{
			switch (ConsumeVoiceType)
			{
			case EXGXunFeiConsumeVoiceType::None:

				break;
			case EXGXunFeiConsumeVoiceType::RealtimeSTT:

				if (UXGXunFeiRealTimeSTTSubsystem::RealTimeSTTSubsystemPtr)
				{
					UXGXunFeiRealTimeSTTSubsystem::RealTimeSTTSubsystemPtr->SendVoiceData(VoiceData);
				}
				
				break;

			
			default:
				
				break;
			
			}
	
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
	UXGXunFeiAudioCaptureSubsystem::ClearVoiceData();

}
