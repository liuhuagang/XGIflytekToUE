// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

#include "XGXunFeiRealTimeSTTRespType.h"

#include "XGXunFeiRealTimeSTTType.generated.h"



DECLARE_DYNAMIC_DELEGATE_FourParams(FXGXunFeiInitRealTimeSTTDelegate, FGuid, AsyncSTTID, bool, bInitResult, FString, InitRespMessage, FXGXunFeiRealTimeSTTInitInformation, RealTimeSTTInitInformation);

DECLARE_DYNAMIC_DELEGATE_OneParam(FXGXunFeiRealTimeSTTRespDelegate, FXGXunFeiRealTimeSTTInformation, RealTimeSTTInformation);

DECLARE_DYNAMIC_DELEGATE_FourParams(FXGXunFeiRealTimeSTTCloseDelegate, FGuid, AsyncSTTID, int32,StatusCode, FString, Reason, bool,bWasClean);



UENUM()
enum class EXGXunFeiRealTimeSTTStatus : uint8
{
	Ready,
	Init,
	Processing,
	WaitToServerClose,
	Finish,
};
