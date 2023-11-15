// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiCoreSettings.h"

FName UXGXunFeiCoreSettings::GetContainerName() const
{
	return TEXT("Project");
}

FName UXGXunFeiCoreSettings::GetCategoryName() const
{
	return TEXT("XGPlugins");
}
FName UXGXunFeiCoreSettings::GetSectionName() const
{
	return TEXT("XunFeiCoreSettings");
}

UXGXunFeiCoreSettings* UXGXunFeiCoreSettings::GetXGXunFeiCoreSettings()
{
	UXGXunFeiCoreSettings* Settings = GetMutableDefault<UXGXunFeiCoreSettings>();

	return Settings;

}
