// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
/**
 * XGXunFeiBaseModule
 */
class FXGXunFeiBaseModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

};
