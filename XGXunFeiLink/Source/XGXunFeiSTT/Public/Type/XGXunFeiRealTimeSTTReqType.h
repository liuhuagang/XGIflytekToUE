// Copyright 2023 Xiao Gang. All Rights Reserved.

/**
 * Be careful:
 * This STT Module only support China region.
 * If you want to use it in other regiion,you can use ASRG Module.
 * 
 * iFlyTek Real Time Speech To Text Document:
 * Chinese:
 * https://www.xfyun.cn/doc/asr/rtasr/API.html
 *
 *
 */
#pragma once
#include "CoreMinimal.h"

#include "XGXunFeiRealTimeSTTReqType.generated.h"


 /**
  * FXGXunFeiRealTimeSTTReqInfo
  * Some RequsetJsonStruct About Real Time SST.
  *
  *
  */
USTRUCT(BlueprintType)
struct XGXUNFEISTT_API FXGXunFeiRealTimeSTTReqInfo
{
	GENERATED_BODY()

protected:

	/**
	 * Parameter:appid
	 * Type:string
	 * Required:Yes
	 * Description:Appid message applied from the platform
	 * Example:5f9c6d6a
	 */
	UPROPERTY()
	FString appid;

	/**
	 * Parameter:ts
	 * Type:string
	 * Required:Yes
	 * Description:Current timestamp, in RFC1123 format
	 * Example:1512041814
	 */
	UPROPERTY()
	FString ts;

	/**
	 * Parameter:signa
	 * Type:string
	 * Required:Yes
	 * Description:Encrypted digital signature (based on HMACSHA1 algorithm)
	 * Example:IrrzsJeOFk1NGfJHW6SkHUoN9CU=
	 */
	UPROPERTY()
	FString signa;

public:

	/**
	 * Parameter:lang
	 * Type:string
	 * Required:No
	 * Description:Real-time speech transfer language.The default is Chinese.
	 * Example:
	 *			Language type：
	 *			Chinese、ChineseAndEnglish：cn；
	 *			Engilis：en；
	 *			Minor languages and dialects can be added from the iFlyTek control panel console.you can get corresponding parameters.
	 *			ParameterExample："lang=en"
	 * Error 10110 is reported,  if unauthorised .
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString lang = TEXT("cn");

	/**
	 * Custom fields Whether to enable the translation function
	 * Note: Translation function needs to be enabled on the iFlyTek control panel console.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	bool bEnableTranslate = false;


	/**
	 * Parameter:transType
	 * Type:string
	 * Required:No
	 * Description:
	 *				"normal" explains general translation. The default is "normal"；
	 * Example:	transType="normal"
	 * Note: Translation function needs to be enabled on the iFlyTek control panel console.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString transType = TEXT("normal");

	/**
	 * Parameter:transType
	 * Type:int
	 * Required:No
	 * Description:
	 *		Strategy 1: Transcribe the VAD results directly to be translated;
	 *		Strategy 2: which returns the results of the intermediate process;
	 *		Strategy 3: Request a translation based on the final punctuation split transcription result;
	 *		Policy 2 is recommended.
	 * Example:
	 *		transStrategy=2
	 * Note: Translation function needs to be enabled on the iFlyTek control panel console.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 transStrategy = 2;


	/**
	 * Parameter:targetLang
	 * Type:string
	 * Required:No
	 * Description:
	 *		Target Languages: Control what type of language the source language is converted to;
	 *		Please note that similar to converting from English to French, Chinese must be used as the transition language, that is, English-Chinese-French, and direct conversion between languages excluding Chinese is not supported.
	 *		Chinese: cn
	 *		English: en
	 *		Japanese: ja
	 *		Korean: ko
	 *		Russian: ru
	 *		French:fr
	 *		Spanish: es
	 *		Vietnamese: vi
	 *		Cantonese: cn_cantonese
	 * Example:
	 *		Such as：targetLang="en"
	 *		If you use Chinese real-time translation to English parameter passing Example is as follows:
	 *		"&lang=cn&transType=normal&transStrategy=2&targetLang=en"
	 * Note: Translation function needs to be enabled on the iFlyTek control panel console.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString targetLang = TEXT("en");

	/**
	 * Parameter:punc
	 * Type:string
	 * Required:No
	 * Description:	Punctuation filter control, punc is returned by default, punc=0 will filter punctuation in the results
	 * Example: 0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString punc = TEXT("None");

	/**
	 * Parameter:pd
	 * Type:string
	 * Required:No
	 * Description:
	 *		Vertical Personalization Parameters:
	 *		Court: court
	 *		Education: edu
	 *		Finance: finance
	 *		Medical: medical
	 *		Technology: tech
	 *		Operator: isp
	 *		Government: gov
	 *		E-Commerce: ecom
	 *		Military: mil
	 *		Enterprise: com
	 *		Life: life
	 *		Car: car
	 * Example:
	 *		Such as ：pd="edu"
	 *		Parameter pd is optional. If no parameter is set, it is common by default
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	FString pd = TEXT("None");


	/**
	 * Parameter:vadMdn
	 * Type:int
	 * Required:No
	 * Description:
	 *			Switch between far and near field, do not pass this parameter or pass 1 for the far field and pass 2 for the near field
	 * Example:	Such as ：vadMdn=2
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 vadMdn = -1;

	/**
	 * Parameter:roleType
	 * Type:int
	 * Required:No
	 * Description:
	 *			Whether to enable character separation, it is not enabled by default, and transmission 2 is enabled
	 *			(the effect is continuously optimized)
	 * Example:	Such as ：roleType=2
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 roleType = -1;

	/**
	 * Parameter:engLangType
	 * Type:int
	 * Required:No
	 * Description:
	 *		Language recognition mode selection, default is mode 1 Chinese and English mode:
	 *		1: Automatic Chinese and English mode
	 *		2: Chinese mode, may include a small amount of English
	 *		4: Pure Chinese mode, does not include English
	 * Example:	Such as ：engLangType=4
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG XunFei RealTime STT")
	int32 engLangType = -1;

public:

	FString GenerateRequireParams();

protected:

	FString GenerateSigna(const FString& InAppid, const FString& InTS, const FString& InAPIKey);

};

