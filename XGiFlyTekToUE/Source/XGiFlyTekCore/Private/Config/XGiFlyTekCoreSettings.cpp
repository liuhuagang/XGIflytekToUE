// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGiFlyTekCoreSettings.h"

FName UXGiFlyTekCoreSettings::GetContainerName() const
{
	return TEXT("Project");
}

FName UXGiFlyTekCoreSettings::GetCategoryName() const
{
	return TEXT("XGPlugins");
}
FName UXGiFlyTekCoreSettings::GetSectionName() const
{
	return TEXT("iFlyTekCoreSettings");
}

UXGiFlyTekCoreSettings* UXGiFlyTekCoreSettings::GetXGiFlyTekCoreSettings()
{
	UXGiFlyTekCoreSettings* Settings = GetMutableDefault<UXGiFlyTekCoreSettings>();

	return Settings;

}
