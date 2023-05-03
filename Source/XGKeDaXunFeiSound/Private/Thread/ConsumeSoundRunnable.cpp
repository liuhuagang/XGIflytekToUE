#include "Thread/ConsumeSoundRunnable.h"
#include "Core/XGKeDaXunFeiSocketSubsystem.h"
#include "Core/XGAudioCaptureSubsystem.h"



FCriticalSection FConsumeSoundRunnable::CriticalSection;
bool FConsumeSoundRunnable::Init()
{
	return true;
}

uint32 FConsumeSoundRunnable::Run()
{
	while (true)
	{

		FScopeLock Lock(&CriticalSection);
		if (!bRunning)
		{
			break;;
		}
		Lock.Unlock();
		FPlatformProcess::Sleep(0.04);



		FScopeLock DataLock(&UXGAudioCaptureSubsystem::XGAudioCriticalSection);
		if (UXGAudioCaptureSubsystem::AudioData.Num()>1024)
		{
	
			TArray<float> SendData;
			SendData.Append(UXGAudioCaptureSubsystem::AudioData.GetData(), 1024);
			UXGAudioCaptureSubsystem::AudioData.RemoveAt(0, 1024);

			UXGKeDaXunFeiSocketSubsystem::SendVoiceData(SendData.GetData(), 1024);
		}
		
	}




	return 0;
}

void FConsumeSoundRunnable::Exit()
{

}

void FConsumeSoundRunnable::Stop()
{
	UE_LOG(LogTemp, Display, TEXT("FConsumeSoundRunnable[%s] begin to stop"), *MyThreadName);
	FScopeLock Lock(&CriticalSection);
	bRunning = false;

}
