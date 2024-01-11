// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

#include "XGXunFeiRealTimeSTTType.generated.h"


DECLARE_DYNAMIC_DELEGATE_TwoParams(FXGXunFeiInitRealTimeSTTDelegate, bool, bInitResult, FString,InitRespMessage);

DECLARE_DYNAMIC_DELEGATE_OneParam(FXGXunFeiRealTimeSTTNoTranslateMiddleDelegate, FString, OutMiddleSrcText);

DECLARE_DYNAMIC_DELEGATE_OneParam(FXGXunFeiRealTimeSTTNoTranslateDelegate, FString, OutSrcText);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FXGXunFeiRealTimeSTTTranslateDelegate, FString, OutSrcText, FString, OutDstText);


UENUM()
enum class EXGXunFeiRealTimeSTTStatus : uint8
{
	Ready,
	Init,
	Processing,
	Finish,
};
