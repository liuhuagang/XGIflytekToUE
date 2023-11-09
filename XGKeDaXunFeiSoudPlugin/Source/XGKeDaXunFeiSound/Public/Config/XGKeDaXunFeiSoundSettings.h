// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "XGKeDaXunFeiSoundSettings.generated.h"


UCLASS(Config = XGKeDaXunFeiSound, defaultconfig)
class  UXGKeDaXunFeiSoundSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override { return TEXT("XGPlugins"); }
	/** The unique name for your section of settings, uses the class's FName. */
	virtual FName GetSectionName() const override { return TEXT("UXGKeDaXunFeiSoundSettings"); }


public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,Category="General")
		FString AppID;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,Category="Real Time Speech To Text")
		FString APIKeySTT;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Text To Speech Stream")
		FString APIKeyTTSStream;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Text To Speech Stream")
		FString APISecretTTSStream;

public:

	UFUNCTION(BlueprintCallable)
		static UXGKeDaXunFeiSoundSettings* GetDataBaseSettings()
	{
		UXGKeDaXunFeiSoundSettings* Settings = GetMutableDefault<UXGKeDaXunFeiSoundSettings>();
		return Settings;

	}


};