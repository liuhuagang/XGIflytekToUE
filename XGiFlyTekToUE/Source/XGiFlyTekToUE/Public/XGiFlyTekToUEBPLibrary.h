// Copyright Xiao Gang. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "XGiFlyTekRealTimeSTTReqType.h"
#include "XGiFlyTekRealTimeSTTType.h"

#include "XGiFlyTekTTSAsyncAction.h"
#include "XGiFlyTekTTSReqType.h"

#include "XGiFlyTekToUEBPLibrary.generated.h"


UCLASS()
class XGIFLYTEKTOUE_API UXGiFlyTekToUEBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * iFlyTekBeginRealTimeSpeechToText
	 *
	 * @param WorldContextObject				WorldContext
	 * @param InRealTimeSTTReqInfo				RealTimeSpeechToTextSettings,you can do nothing to keep default.
	 * Default lanaguge is Chinese,you can change to Chinese to Enginsh,such as "cn"->"en". Default No Translate,just Src.if you need to translate,you need to open it function in iFlyTek web.
	 * @param InInitRealTimeSTTDelegate			Tell you whether to success to create socket to iFlyTek and begin to audio capture.
	 * if your app key and id are not right,you will fail.if your audio input device is not good,you will fail.
	 * Suggest audio input [48000 kHz,16 bits, 2 channle][48000 kHz,16 bits, 1 channle][16000 kHz,16 bits, 1 channle],thest can work well.
	 * @param InRealTimeSTTNoTranslateDelegate	Give you Src Speech to Text .
	 * @param InRealTimeSTTTranslateDelegate	Give you Src Speech to Text Src and Target Translate Text.
	 * If your app Id support and you open it in this plugin.More to look iFlyTekDoc.
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "iFlyTekBeginRealTimeSpeechToText", WorldContext = "WorldContextObject",
		Keywords = "XG iFlyTek STT SPeech To Text "), Category = "XGiFlyTekToUE|STT")
	static void XGiFlyTekBeginRealTimeSpeechToText(const UObject* WorldContextObject,
		FXGiFlyTekRealTimeSTTReqInfo InRealTimeSTTReqInfo,
		FXGiFlyTekInitRealTimeSTTDelegate InInitRealTimeSTTDelegate,
		FXGiFlyTekRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate,
		FXGiFlyTekRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate);


	/**
	 *
	 * iFlyTekStopRealTimeSpeechToText
	 *
	 * @param WorldContextObject WorldContext
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "iFlyTekStopRealTimeSpeechToText", WorldContext = "WorldContextObject",
		Keywords = "XG iFlyTek STT SPeech To Text "), Category = "XGiFlyTekToUE|STT")
	static void XGiFlyTekStopRealTimeSpeechToText(const UObject* WorldContextObject);


	/**
	 *
	 *
	 * @param WorldContextObject		WorldContext
	 * @param InText					The Str you want to Convert to Audio
	 * @param bInSaveToLocal			Whether to Save to local disk
	 * @param InSaveFileFullPath		The FilePath which the wmv file will save to .
	 * This directory must exist,and the file name must end with ".wav"
	 * This path is absoult path!
	 * @param IniFlyTekTTSReqInfo		About TTS Settins,more to look iflytesk document.you can choos which voice to say,and so on.
	 * @param OniFlyTekTTSSuccess		if success ,you will get USoundWave.
	 * @param OniFlyTekTTSFail			if faile,you will get reason.
	 */
	static void XGiFlyTekTextToSpeech(UObject* WorldContextObject,
		const FString& InText,
		bool bInSaveToLocal,
		const FString& InSaveFileFullPath,
		FXGiFlyTekTTSReqInfo IniFlyTekTTSReqInfo,
		FXGiFlyTekTTSDelegate OniFlyTekTTSSuccess,
		FXGiFlyTekTTSDelegate OniFlyTekTTSFail);

};
