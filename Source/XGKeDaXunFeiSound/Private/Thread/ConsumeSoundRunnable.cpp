#include "Thread/ConsumeSoundRunnable.h"



 FCriticalSection FConsumeSoundRunnable::CriticalSection;
bool FConsumeSoundRunnable::Init()
{
	return true;
}

uint32 FConsumeSoundRunnable::Run()
{
	while (bRunning)
	{
		FScopeLock Lock(&CriticalSection);

		FPlatformProcess::Sleep(0.2);
		UE_LOG(LogTemp, Display, TEXT("heelo[%s]"), *MyThreadName);
	}




	return 0;
}

void FConsumeSoundRunnable::Exit()
{

}

void FConsumeSoundRunnable::Stop()
{
	UE_LOG(LogTemp, Display, TEXT("heelo[%s]stopsssssss"), *MyThreadName);
	FScopeLock Lock(&CriticalSection);
	bRunning = false;

}
