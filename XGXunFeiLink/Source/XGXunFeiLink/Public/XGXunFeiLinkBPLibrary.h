// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "XGXunFeiRealTimeSTTReqType.h"
#include "XGXunFeiRealTimeSTTType.h"

#include "XGXunFeiTTSAsyncAction.h"
#include "XGXunFeiTTSReqType.h"



#include "XGXunFeiLinkBPLibrary.generated.h"


class USoundWave;
/**
 * UXGXunFeiLinkBPLibrary
 * Exposed to users.
 * You can use these methods.
 */
UCLASS()
class XGXUNFEILINK_API UXGXunFeiLinkBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * XunFeiBeginRealTimeSpeechToText
	 *
	 * @param WorldContextObject				WorldContext
	 * @param InRealTimeSTTReqInfo				RealTimeSpeechToTextSettings,you can do nothing to keep default.
	 * Default lanaguge is Chinese,you can change to Chinese to Enginsh,such as "cn"->"en". Default No Translate,just Src.if you need to translate,you need to open it function in XunFei web.
	 * @param InInitRealTimeSTTDelegate			Tell you whether to success to create socket to XunFei and begin to audio capture.
	 * if your app key and id are not right,you will fail.if your audio input device is not good,you will fail.
	 * Suggest audio input [48000 kHz,16 bits, 2 channle][48000 kHz,16 bits, 1 channle][16000 kHz,16 bits, 1 channle],thest can work well.
	 * @param InRealTimeSTTNoTranslateDelegate	Give you Src Speech to Text .
	 * @param InRealTimeSTTTranslateDelegate	Give you Src Speech to Text Src and Target Translate Text.
	 * If your app Id support and you open it in this plugin.More to look XunFeiDoc.
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XunFeiBeginRealTimeSpeechToText", WorldContext = "WorldContextObject",
		Keywords = "XG XunFei STT SPeech To Text "), Category = "XGXunFeiLink|STT")
	static void XGXunFeiBeginRealTimeSpeechToText(const UObject* WorldContextObject,
		FXGXunFeiRealTimeSTTReqInfo InRealTimeSTTReqInfo,
		FXGXunFeiInitRealTimeSTTDelegate InInitRealTimeSTTDelegate,
		FXGXunFeiRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate,
		FXGXunFeiRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate);


	/**
	 *
	 * XunFeiStopRealTimeSpeechToText
	 *
	 * @param WorldContextObject WorldContext
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XunFeiStopRealTimeSpeechToText", WorldContext = "WorldContextObject",
		Keywords = "XG XunFei STT SPeech To Text "), Category = "XGXunFeiLink|STT")
	static void XGXunFeiStopRealTimeSpeechToText(const UObject* WorldContextObject);


	/**
	 *
	 *
	 * @param WorldContextObject		WorldContext
	 * @param InText					The Str you want to Convert to Audio
	 * @param bInSaveToLocal			Whether to Save to local disk
	 * @param InSaveFileFullPath		The FilePath which the wmv file will save to .
	 * This directory must exist,and the file name must end with ".wav"
	 * This path is absoult path!
	 * @param InXunFeiTTSReqInfo		About TTS Settins,more to look iflytesk document.you can choos which voice to say,and so on.
	 * @param OnXunFeiTTSSuccess		if success ,you will get USoundWave.
	 * @param OnXunFeiTTSFail			if faile,you will get reason.
	 */
	static void XGXunFeiTextToSpeech(UObject* WorldContextObject,
		const FString& InText,
		bool bInSaveToLocal,
		const FString& InSaveFileFullPath,
		FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo,
		FXGXunFeiTTSDelegate OnXunFeiTTSSuccess,
		FXGXunFeiTTSDelegate OnXunFeiTTSFail);





};
