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

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RealTime AutomatedSpeechRecognition Global")
	FString App_ID_RealTimeASR_Global;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RealTime AutomatedSpeechRecognition Global")
	FString API_Secret_RealTimeASR_Global;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RealTime AutomatedSpeechRecognition Global")
	FString API_Key_RealTimeASR_Global;




	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Online TextToSpeech Global")
	FString App_ID_OnlineTTS_Global;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Online TextToSpeech Global")
	FString API_Secret_OnlineTTS_Global;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Online TextToSpeech Global")
	FString API_Key_OnlineTTS_Global;





	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RealTime SpeechToText")
	FString App_ID_RealTimeSTT;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RealTime SpeechToText")
	FString API_Key_RealTimeSTT;


	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech Stream")
	FString App_ID_TTSStream;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech Stream")
	FString API_Secret_TTSStream;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech Stream")
	FString API_Key_TTSStream;




	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spark")
	FString App_ID_Spark;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spark")
	FString API_Secret_Spark;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spark")
	FString API_Key_Spark;







};