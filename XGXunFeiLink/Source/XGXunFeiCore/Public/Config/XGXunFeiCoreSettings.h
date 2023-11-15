// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "XGXunFeiCoreSettings.generated.h"

/**
 * UXGXunFeiCoreSettings
 * Input iFlyTek App Information
 */
UCLASS(Config = XGXunFeiCoreSettings, defaultconfig)
class XGXUNFEICORE_API UXGXunFeiCoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	virtual FName GetContainerName() const override;

	virtual FName GetCategoryName() const override;

	virtual FName GetSectionName() const override;

public:

	static UXGXunFeiCoreSettings* GetXGXunFeiCoreSettings();

public:

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "XunFei Common")
	FString AppID;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Real Time Speech To Text")
	FString APIKeyRealTimeSTT;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Text To Speech Stream")
	FString APIKeyTTSStream;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Text To Speech Stream")
	FString APISecretTTSStream;




};