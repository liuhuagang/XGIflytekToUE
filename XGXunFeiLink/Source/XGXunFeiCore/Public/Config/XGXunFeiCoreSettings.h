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

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RealTime SpeechToText")
	FString AppID_RealTimeSTT;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RealTime SpeechToText")
	FString APIKey_RealTimeSTT;


	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech Stream")
	FString AppID_TTSStream;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech Stream")
	FString APIKey_TTSStream;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech Stream")
	FString APISecret_TTSStream;


	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spark")
	FString AppID_Spark;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spark")
	FString APIKey_Spark;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spark")
	FString APISecret_Spark;





};