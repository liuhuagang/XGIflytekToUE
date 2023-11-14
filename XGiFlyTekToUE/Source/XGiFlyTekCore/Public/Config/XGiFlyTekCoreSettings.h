// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "XGiFlyTekCoreSettings.generated.h"


UCLASS(Config = XGiFlyTekCoreSettings, defaultconfig)
class XGIFLYTEKCORE_API UXGiFlyTekCoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	virtual FName GetContainerName() const override;

	virtual FName GetCategoryName() const override;

	virtual FName GetSectionName() const override;

public:

	UFUNCTION(BlueprintCallable, Category = "XGiFlyTekCoreSettings")
	static UXGiFlyTekCoreSettings* GetXGiFlyTekCoreSettings();

public:

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "General")
	FString AppID;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Real Time Speech To Text")
	FString APIKeySTT;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Text To Speech Stream")
	FString APIKeyTTSStream;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Text To Speech Stream")
	FString APISecretTTSStream;




};