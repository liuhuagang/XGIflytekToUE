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
	 * @param WorldContextObject						WorldContext
	 * @param InRealTimeSTTReqInfo						RealTimeSpeechToTextSettings,you can do nothing to keep default.
	 *													Default lanaguge is Chinese,you can change to Chinese to Enginsh,such as "cn"->"en". Default No Translate,just Src.if you need to translate,you need to open it function in XunFei web.
	 *
	 * @param InSTTAppID								iFlyTek AppID
	 * @param InSTTAPIKey								iFlyTek APIKey
	 * 
	 * @param InInitRealTimeSTTDelegate					Tell you whether to success to create socket to XunFei and begin to audio capture.
	 *													if your app key and id are not right,you will fail.if your audio input device is not good,you will fail.
	 *													Suggest audio input [48000 kHz,16 bits, 2 channle][48000 kHz,16 bits, 1 channle][16000 kHz,16 bits, 1 channle],thest can work well.
	 *
	 * @param InRealTimeSTTNoTranslateMiddleDelegate	Give you the middle Src Speech to Text .
	 * @param InRealTimeSTTNoTranslateDelegate			Give you Src Speech to Text .
	 * @param InRealTimeSTTTranslateDelegate			Give you Src Speech to Text Src and Target Translate Text.
	 *
	 * If your app ID supports to translage and you open it in this plugin.More to look XunFeiDoc.
	 *
	 *
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XunFeiBeginRealTimeSpeechToText", WorldContext = "WorldContextObject",
		Keywords = "XG XunFei STT SPeech To Text "), Category = "XGXunFeiLink|STT")
	static void XGXunFeiBeginRealTimeSpeechToText(const UObject* WorldContextObject,
		FString InSTTAppID,
		FString InSTTAPIKey,
		FXGXunFeiRealTimeSTTReqInfo InRealTimeSTTReqInfo,
		FXGXunFeiInitRealTimeSTTDelegate InInitRealTimeSTTDelegate,
		FXGXunFeiRealTimeSTTNoTranslateMiddleDelegate InRealTimeSTTNoTranslateMiddleDelegate,
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
	 * XGXunFeiTextToSpeech
	 *
	 * @param WorldContextObject			WorldContext
	 *
	 * @param InTTSAppID					iFlyTek AppID
	 * @param InTTSAPISecret				iFlyTek APISecret
	 * @param InTTSAPIKey					iFlyTek APIKey
	 *
	 * @param InText						The Str you want to Convert to Audio
	 * @param bInSaveToLocal				Whether to Save to local disk
	 * @param InSaveFileFullPath			The FilePath which the wmv file will save to .
	 *										This directory must exist,and the file name must end with ".wav"
	 *										This path is absoult path!
	 * @param InXunFeiTTSReqInfo			About TTS Settins,more to look iflytesk document.you can choos which voice to say,and so on.
	 *
	 * @param OnXunFeiTTSSoundWaveSuccess	If it succeeds ,you will get USoundWave.
	 * @param OnXunFeiTTSSoundWaveFail		If it fails,you will get reasons.
	 * @param OnXunFeiTTSWavFileSuccess		If it succeeds ,you will get sound file.
	 * @param OnXunFeiTTSWavFileFail		If it fails,you will get reasons.
	 * 
	 *
	 * @return FGuid					The AsyncID.
	 */
	static FGuid XGXunFeiTextToSpeech(UObject* WorldContextObject,
		FString InTTSAppID,
		FString InTTSAPISecret,
		FString InTTSAPIKey,
		const FString& InText,
		bool bInSaveToLocal,
		const FString& InSaveFileFullPath,
		FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo,
		FXGXunFeiTTSDelegate OnXunFeiTTSSoundWaveSuccess,
		FXGXunFeiTTSDelegate OnXunFeiTTSSoundWaveFail,
		FXGXunFeiTTSDelegate OnXunFeiTTSWavFileSuccess,
		FXGXunFeiTTSDelegate OnXunFeiTTSWavFileFail);




};
