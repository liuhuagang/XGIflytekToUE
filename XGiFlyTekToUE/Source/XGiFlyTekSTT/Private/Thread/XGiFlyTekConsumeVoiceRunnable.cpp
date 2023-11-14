#include "XGiFlyTekConsumeVoiceRunnable.h"
#include "XGiFlyTekAudioCaptureSubsystem.h"
#include "XGiFlyTekRealTimeSTTSubsystem.h"
FXGiFlyTekConsumeVoiceRunnable::FXGiFlyTekConsumeVoiceRunnable(FString InThreadName,
	UXGiFlyTekAudioCaptureSubsystem* InAudioCaptureSubsystem,
	UXGiFlyTekRealTimeSTTSubsystem* InRealTimeSTTSubsystem)
:ThreadName(InThreadName)
, AudioCaptureSubsystem(InAudioCaptureSubsystem)
, RealTimeSTTSubsystem(InRealTimeSTTSubsystem)
{


}

FXGiFlyTekConsumeVoiceRunnable::~FXGiFlyTekConsumeVoiceRunnable()
{

}

bool FXGiFlyTekConsumeVoiceRunnable::Init()
{
	return true;
}

uint32 FXGiFlyTekConsumeVoiceRunnable::Run()
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

void FXGiFlyTekConsumeVoiceRunnable::Stop()
{
	bIsRunning = false;

}

void FXGiFlyTekConsumeVoiceRunnable::Exit()
{
	AudioCaptureSubsystem->ClearVoiceData();
}
