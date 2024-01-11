// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiCoreSettings.h"
#include "Interfaces/IPluginManager.h"
#include "PluginDescriptor.h"


UXGXunFeiCoreSettings::UXGXunFeiCoreSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	IPluginManager& PluginManager = IPluginManager::Get();

	TSharedPtr<IPlugin> PluginPtr = PluginManager.FindPlugin(TEXT("XGXunFeiLink"));

	if (PluginPtr.IsValid())
	{
		FPluginDescriptor PluginDescriptor = PluginPtr->GetDescriptor();
		PluginVersion = PluginDescriptor.VersionName;
	}


	
	ASRG.FunctionName = TEXT("ASRG");
	ASRG.MethodName = TEXT("XunFeiBeginRealTimeASRGlobal and XGXunFeiStopAutomatedSpeechRecognitionGlobal");
	ASRG.Description = TEXT("Real-time ASR based on the deep fully convolutional neural network framework (DFCNN), converting audio stream data (within 5 hours) into text stream data results in real-time. It provides a basis for information processing and data mining, which is more suitable for daily dialogue.");
	ASRG.WebURL = TEXT("https://global.xfyun.cn/doc/rtasr/rtasr/API.html");
	ASRG.bGlobal = true;

	TTSG.FunctionName = TEXT("TTSG");
	TTSG.MethodName = TEXT("XGXunFeiTextToSpeechGlobal");
	TTSG.Description = TEXT("The stream interface for Online TTS converts text information into acoustic information, and provides a number of distinctive speakers (acoustic library) for you to choose from. Click here to experience the speech effect online. This function is a common interface provided to developers through Websocket API. Websocket API can achieve streaming, applicable to AI service scenarios that require streaming data transmission. Compared to SDK, API is featured as lightweight and cross-language; compared to the HTTP API, the Websocket API protocol has the advantage of natively supporting cross-domain.");
	TTSG.WebURL = TEXT("https://global.xfyun.cn/doc/tts/online_tts/API.html");
	TTSG.bGlobal = true;

	STT.FunctionName = TEXT("STT");
	STT.MethodName = TEXT("XunFeiBeginRealTimeSpeechToText and XunFeiStopRealTimeSpeechToText");
	STT.Description = TEXT("");
	STT.WebURL = TEXT("https://www.xfyun.cn/doc/asr/rtasr/API.html");

	TTS.FunctionName = TEXT("TTS");
	TTS.MethodName = TEXT("XunFeiTextToSpeech");
	TTS.Description = TEXT("");
	TTS.WebURL = TEXT("https://www.xfyun.cn/doc/tts/online_tts/API.html");

	Spark.FunctionName = TEXT("Spark");
	Spark.MethodName = TEXT("XunFeiSpark");
	Spark.Description = TEXT("");
	Spark.WebURL = TEXT("https://www.xfyun.cn/doc/spark/Web.html");

	SparkHelper.FunctionName = TEXT("SparkHelper");
	SparkHelper.MethodName = TEXT("XunFeiSparkHelper");
	SparkHelper.Description = TEXT("");
	SparkHelper.WebURL = TEXT("https://www.xfyun.cn/doc/spark/SparkAssistantAPI.html");

	SparkHint.FunctionName = TEXT("SparkHint");
	SparkHint.MethodName = TEXT("XunFeiSparkHint");
	SparkHint.Description = TEXT("");
	SparkHint.WebURL = TEXT("https://www.xfyun.cn/doc/spark/%E6%98%9F%E7%81%AB%E5%BE%AE%E8%B0%83%E6%9C%8D%E5%8A%A1Web%E6%96%87%E6%A1%A3.html");

	SparkV3FunctionCall.FunctionName = TEXT("SparkV3FunctionCall");
	SparkV3FunctionCall.MethodName = TEXT("XGXunFeiSparkV3FunctionCall");
	SparkV3FunctionCall.Description = TEXT("");
	SparkV3FunctionCall.WebURL = TEXT("https://www.xfyun.cn/doc/spark/Web.html");

	ImgUnderstand.FunctionName = TEXT("ImgUnderstand");
	ImgUnderstand.MethodName = TEXT("XunFeiSparkImgUnderstand,XunFeiLoadPitcureFileToBinaryData");
	ImgUnderstand.Description = TEXT("");
	ImgUnderstand.WebURL = TEXT("https://www.xfyun.cn/doc/spark/ImageUnderstanding.html");



}

UXGXunFeiCoreSettings::~UXGXunFeiCoreSettings()
{
}

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
