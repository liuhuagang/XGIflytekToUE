// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

#include "XGXunFeiRealTimeSTTRespType.generated.h"

/**
 * FXGXunFeiRealTImeSTTRespInfo
 */
USTRUCT(BlueprintType)
struct FXGXunFeiRealTImeSTTRespInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString action = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString code = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString data = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString desc = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString sid = TEXT("");
};

/**
 * FXGXunFeiRealTimeSTTTranslateData
 */
USTRUCT(BlueprintType)
struct FXGXunFeiRealTimeSTTTranslateData
{
	GENERATED_BODY()

	/**
	 * Service identification field. The value is trans after the translation function is enabled
	 * Translation function identification
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString biz = TEXT("");

	/**
	 * Translated text results for the target language
	 * Corresponds to the original text src
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString dst = TEXT("");
	/**
	 * End of translation mark
	 * If true, the translation result has been pushed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	bool isEnd =false;

	/**
	 * Transliteration result sequence number
	 * Start at 0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 segId = -1;

	/**
	 * Send translated original text
	 * The audio corresponds to the identifying text
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString src = TEXT("");

	/**
	 * Result Type identification
	 * 0- Final result; 1- Intermediate result
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 type = -1;


	/**
	 * The beginning time of a sentence in the whole speech, in milliseconds (ms)
	 * The bg of the intermediate result is the exact value
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 bg=-1;


	/**
	 * The end time of the sentence in the whole speech, in milliseconds (ms)
	 * The ed of the intermediate result is 0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 ed=-1;

};

/**
 * FXGXunFeiRealTimeSTTNoTranslateCWData
 */
USTRUCT(BlueprintType)
struct FXGXunFeiRealTimeSTTNoTranslateCWData
{
	GENERATED_BODY()

	/**
	 * Word recognition result
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString w = TEXT("");

	/**
	 * Word recognition result
	 * n- common words; S-smooth word (modal word); p-punctuation
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString wp = TEXT("");
};

/**
 * FXGXunFeiRealTimeSTTNoTranslateWSData
 */
USTRUCT(BlueprintType)
struct FXGXunFeiRealTimeSTTNoTranslateWSData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	TArray<FXGXunFeiRealTimeSTTNoTranslateCWData> cw;
	/**
	 * The start time of the word in this sentence, in frames, 1 frame =10ms
	 * The start time of the word in the whole speech is (bg+wb*10)ms
	 * The wb of the intermediate result is 0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 wb=-1;

	/**
	* The end time of the word in this sentence, in frames, 1 frame =10ms
	* The end time of the word in the whole speech is (bg+we*10)ms
	* The we of the intermediate result is 0
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 we=-1;
};


/**
 * FXGXunFeiRealTimeSTTNoTranslateRTData
 */
USTRUCT(BlueprintType)
struct FXGXunFeiRealTimeSTTNoTranslateRTData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	TArray<FXGXunFeiRealTimeSTTNoTranslateWSData> ws;
};




/**
 * FXGXunFeiRealTimeSTTNoTranslateSTData
 */
USTRUCT(BlueprintType)
struct FXGXunFeiRealTimeSTTNoTranslateSTData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString bg = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString ed = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	TArray<FXGXunFeiRealTimeSTTNoTranslateRTData> rt;


	/**
	 * Result Type identifier
	 *  0- Final result; 1- Intermediate result
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString type = TEXT("");
};


/**
 * FXGXunFeiRealTimeSTTNoTranslateCNData
 */
USTRUCT(BlueprintType)
struct FXGXunFeiRealTimeSTTNoTranslateCNData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FXGXunFeiRealTimeSTTNoTranslateSTData st;
};

/**
 * FXGXunFeiRealTimeSTTNoTranslateData
 */
USTRUCT(BlueprintType)
struct FXGXunFeiRealTimeSTTNoTranslateData
{
	GENERATED_BODY()

	/**
	 * Presumed to be a regional code
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FXGXunFeiRealTimeSTTNoTranslateCNData cn;


	/**
	 * Transcribe the result sequence number
	 * Start at 0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 seg_id=-1;
};
