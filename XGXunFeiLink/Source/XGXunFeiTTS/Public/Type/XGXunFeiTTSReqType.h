// Copyright 2023 Xiao Gang. All Rights Reserved.

/**
 * 
 * Be careful:
 * This TTS Module only support China region.
 * If you want to use it in other regiion,you can use TTSG Module.
 * iFlyTek Text To Speech Steam Document:
 * Chinese:
 * https://www.xfyun.cn/doc/tts/online_tts/API.html
 * 
 */


#pragma once
#include "CoreMinimal.h"

#include "XGXunFeiTTSReqType.generated.h"



 /**
  *	FXGXunFeiTTSReqCommonInfo
  */
USTRUCT()
struct XGXUNFEITTS_API FXGXunFeiTTSReqCommonInfo
{
	GENERATED_BODY()


public:
	/**
	 * Parameter:appid
	 * Type:string
	 * Required:Yes
	 * Description:Appid message applied from the platform
	 * Example:5f9c6d6a
	 */
	UPROPERTY()
	FString app_id = TEXT("");

};

/**
 *	FXGXunFeiTTSReqBusinessInfo
 */
USTRUCT(BlueprintType)
struct XGXUNFEITTS_API FXGXunFeiTTSReqBusinessInfo
{
	GENERATED_BODY()


public:
	/**
	 * Parameter:aue
	 * Type:string
	 * Required:Yes
	 * Description:
	 *		Audio coding, optional values:
	 *		raw：Uncompressed pcm
	 *		lame：mp3 (when aue=lame,required: sfl=1)
	 *		speex-org-wb;7： Open source standards speex（for speex_wideband，16k）
	 *		The number represents the specified compression level (the default level is 8)

	 *		speex-org-nb;7： Open source standards speex（for speex_narrowband，8k）
	 *		The number represents the specified compression level (the default level is 8)

	 *		speex;7：Standard open source compression format, compression level 1~10, default 7 (8k IFlytek Custom speex)
	 *		speex-wb;7：Compression format, compression level 1~10, default is 7 (16k IFlytek Custom speex)
	 * Example:
	 *		"raw"
	 *		"speex-org-wb;7" ，The number Required
	 *		The number represents the specified compression level (the default level is 8)
	 * 
	 * 
	 *		Open source standards speex code and iFlyTek  speexDescription ,please refer to Audio Description
	 * 
	 * Only Support "raw".
	 */
	UPROPERTY()
	FString aue = TEXT("raw");



	/**
	 * Parameter:sfl
	 * Type:int
	 * Required:No
	 * Description:
	 *		 When aue=lame，Turn on stream return
	 *		mp3，audio format 
	 *		value：1 ,Turn on.
	 *
	 * Example:1
	 * This plug-in does not use this value

	 */
	 //UPROPERTY()
	int32 sfl = -1;

	/**
	 * Parameter:auf
	 * Type:string
	 * Required:No
	 * Description:
	 * Audio sampling rate, optional values:
	 *		audio/L16;rate=8000：Synthesize 8K audio
	 *		audio/L16;rate=16000：Synthesize 16K audio
	 *		auf No-pass value：Synthesize 16K audio
	 * Example:
	 *		"audio/L16;rate=16000"
	 *
	 */
	UPROPERTY()
	FString auf = TEXT("audio/L16;rate=16000");



	/**
	 * Parameter:vcn
	 * Type:string
	 * Required:Yes
	 * Description:
	 *		Pronunciation person, optional value: 
	 *		"xiaoyan",
	 *		"aisjiuxu",
	 *		"aisxping",
	 *		"aisjinger",
	 *		"aisbabyxu"
	 *		Please go to the console to add a trial or purchase pronunciation person, after adding the pronunciation person Parameter value will be displayed
	 * Example:
	 *		"xiaoyan"
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	FString vcn = TEXT("xiaoyan");

	/**
	 * Parameter:speed
	 * Type:int
	 * Required:No
	 * Description:
	 *		The value is optional: [0-100]. The default value is 50
	 * Example:
	 *		50
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	int32 speed = 50;

	/**
	 * Parameter:volume
	 * Type:int
	 * Required:No
	 * Description:
	 *		The value is optional: [0-100]. The default value is 50
	 * Example:
	 *		50
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	int32 volume = 50;

	/**
	 * Parameter:pitch
	 * Type:int
	 * Required:No
	 * Description:
	 *		The value is optional: [0-100]. The default value is 50
	 * Example:
	 *		50
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	int32 pitch = 50;

	/**
	 * Parameter:bgs
	 * Type:int
	 * Required:No
	 * Description:
	 *		Background sound for synthesized audio
	 *			0: No background sound (default)
	 *			1: Background sound
	 * Example:
	 *		0
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	int32 bgs = 0;


	/**
	 * Parameter:tte
	 * Type:string
	 * Required:No
	 * Description:
	 *			Text encoding format
	 *			GB2312
	 *			GBK
	 *			BIG5
	 *			UNICODE (UNICODE encoding is Required for minor languages, and utf16 encoding is required for synthesized text. See javaExampledemo for details.)
	 *			GB18030
	 *			UTF8（minority language）
	 * Example:
	 *		"UTF8"
	 *
	 */
	UPROPERTY()
	FString tte = TEXT("UTF8");

	/**
	 * Parameter:reg
	 * Type:string
	 * Required:No
	 * Description:
	 *		Set English pronunciation:
	 *			0: Automatic judgment processing, if not sure will be processed according to the English word spelling (default)
	 *			1: All English is pronounced alphabetically
	 *			2: Automatic judgment processing, if not sure will be read according to the letter
	 *		Default pronunciation according to English words
	 * Example:
	 *		"2"
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	FString reg = TEXT("2");

	/**
	 * Parameter:rdn
	 * Type:string
	 * Required:No
	 * Description:
	 *		Synthetic audio digital pronunciation
	 *			0: Automatic judgment (default)
	 *			1: full value
	 *			2: complete string
	 *			3: The string takes precedence
	 * Example:		"0"
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	FString rdn = TEXT("0");



};



/**
 *	FXGXunFeiTTSReqDataInfo
 */
USTRUCT()
struct XGXUNFEITTS_API FXGXunFeiTTSReqDataInfo
{
	GENERATED_BODY()


public:
	/**
	 * Parameter:text
	 * Type:string
	 * Required:No
	 * Description:
	 *		The text content needs to be base64 encoded;
	 *		The maximum length before base64 encoding must be less than 8000 bytes, about 2000 Chinese characters
	 *			
	 *
	 */
	UPROPERTY()
	FString text = TEXT("");


	/**
	 * Parameter:text
	 * Type:int
	 * Required:Yes
	 * Description:
	 *		Data status, fixed to 2
	 *		Note: Since the text of stream synthesis can only be transmitted once, and multiple segmented transmission is not supported, statusRequired is 2.
	 *
	 */
	UPROPERTY()
	int32 status = 2;


};




/**
 * FXGXunFeiTTSReqInfo
 */
USTRUCT(BlueprintType)
struct XGXUNFEITTS_API FXGXunFeiTTSReqInfo
{
	GENERATED_BODY()

	friend class UXGXunFeiTTSAsyncAction;

public:
	/**
	 * Parameter:business
	 * Type:object
	 * Required:Yes
	 * Description:
	 *		Service parameter.
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	FXGXunFeiTTSReqBusinessInfo business;

protected:
	/**
	 * Parameter:common
	 * Type:object
	 * Required:Yes
	 * Description:
	 *		Common parameter.
	 *
	 */
	UPROPERTY()
	FXGXunFeiTTSReqCommonInfo common;
	/**
	 * Parameter:data
	 * Type:object
	 * Required:Yes
	 * Description:
	 *		Service data flow parameters.
	 *
	 */
	UPROPERTY()
	FXGXunFeiTTSReqDataInfo data;

};