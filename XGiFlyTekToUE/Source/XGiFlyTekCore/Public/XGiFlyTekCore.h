// Copyright Xiao Gang. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FXGiFlyTekCoreModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
