// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "XGXunFeiCoreSettings.generated.h"


USTRUCT(BlueprintType)
struct FXunFeiFunctionDescriber
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Base")
	FString FunctionName = TEXT("None");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	FString MethodName = TEXT("None");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	FString Description = TEXT("None");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "URL")
	FString WebURL = TEXT("None");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Region")
	bool bGlobal = false;


};




/**
 * UXGXunFeiCoreSettings
 * Input iFlyTek App Information
 */
UCLASS(Config = XGXunFeiCoreSettings, defaultconfig)
class XGXUNFEICORE_API UXGXunFeiCoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UXGXunFeiCoreSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGXunFeiCoreSettings();

	virtual FName GetContainerName() const override;

	virtual FName GetCategoryName() const override;

	virtual FName GetSectionName() const override;

public:

	static UXGXunFeiCoreSettings* GetXGXunFeiCoreSettings();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	FString PluginVersion = TEXT("");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportASRG = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportTTSG = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportSTT = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportTTS = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportSpark = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportSparkHelper = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportSparkHint = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportSparkV3FunctionCall = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkInfo")
	bool bSupportImgUnderstand = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction")
	FXunFeiFunctionDescriber ASRG;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction")
	FXunFeiFunctionDescriber TTSG;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction")
	FXunFeiFunctionDescriber STT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction")
	FXunFeiFunctionDescriber TTS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction|Spark")
	FXunFeiFunctionDescriber Spark;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction|Spark")
	FXunFeiFunctionDescriber SparkHelper;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction|Spark")
	FXunFeiFunctionDescriber SparkHint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction|Spark")
	FXunFeiFunctionDescriber SparkV3FunctionCall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XunFeiLinkFunction|Spark")
	FXunFeiFunctionDescriber ImgUnderstand;







};