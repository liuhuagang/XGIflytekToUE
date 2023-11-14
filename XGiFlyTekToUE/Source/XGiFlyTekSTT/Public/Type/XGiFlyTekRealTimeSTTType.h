// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

#include "XGiFlyTekRealTimeSTTType.generated.h"


DECLARE_DYNAMIC_DELEGATE_TwoParams(FXGiFlyTekInitRealTimeSTTDelegate, bool, bInitResult, FString,InitRespMessage);

DECLARE_DYNAMIC_DELEGATE_OneParam(FXGiFlyTekRealTimeSTTNoTranslateDelegate, FString, OutSrcText);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FXGiFlyTekRealTimeSTTTranslateDelegate, FString, OutSrcText, FString, OutDstText);


UENUM()
enum class EXGiFlyTekRealTimeSTTStatus : uint8
{
	Ready,
	Init,
	Processing,
	Finish,
};
