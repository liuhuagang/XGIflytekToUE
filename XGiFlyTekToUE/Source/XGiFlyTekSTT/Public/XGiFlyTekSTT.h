// Copyright Xiao Gang. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FXGiFlyTekSTTModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
