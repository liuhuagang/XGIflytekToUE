// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiCoreAsyncAction.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
UXGXunFeiCoreAsyncAction::UXGXunFeiCoreAsyncAction(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	AsyncID(FGuid::NewGuid())
{


}

void UXGXunFeiCoreAsyncAction::Activate()
{
	Super::Activate();

	AsyncTask(ENamedThreads::GameThread, [this]() {

		this->Activate_Internal();

		});

}

void UXGXunFeiCoreAsyncAction::Activate_Internal()
{


}
