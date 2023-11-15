// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

class FConsumeSoundRunnable :public FRunnable
{
public:
	FConsumeSoundRunnable(FString ThreadName) :MyThreadName(ThreadName)
	{

	}

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;

	FString MyThreadName;




	static FCriticalSection CriticalSection;
protected:
	bool bRunning = true;

};