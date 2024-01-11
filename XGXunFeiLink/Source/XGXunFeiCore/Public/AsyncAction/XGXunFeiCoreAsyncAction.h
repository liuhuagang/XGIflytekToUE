// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "XGXunFeiCoreAsyncAction.generated.h"


UCLASS()
class XGXUNFEICORE_API UXGXunFeiCoreAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:

	UXGXunFeiCoreAsyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Activate() override ;


	virtual void Activate_Internal() ;

public:

	FGuid AsyncID;
};