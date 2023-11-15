// Copyright 2023 Xiao Gang. All Rights Reserved.

/**
 * iFlyTek Text To Speech Steam Document:
 * https://www.xfyun.cn/doc/tts/online_tts/API.html
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
	UPROPERTY()
	FString app_id;

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
	 * 参数名:aue
	 * 类型:string
	 * 必传:是
	 * 描述:
	 *		音频编码，可选值：
	 *		raw：未压缩的pcm
	 *		lame：mp3 (当aue=lame时需传参sfl=1)
	 *		speex-org-wb;7： 标准开源speex（for speex_wideband，即16k）数字代表指定压缩等级（默认等级为8）
	 *		speex-org-nb;7： 标准开源speex（for speex_narrowband，即8k）数字代表指定压缩等级（默认等级为8）
	 *		speex;7：压缩格式，压缩等级1~10，默认为7（8k讯飞定制speex）
	 *		speex-wb;7：压缩格式，压缩等级1~10，默认为7（16k讯飞定制speex）
	 * 示例:
	 *		"raw"
	 *		"speex-org-wb;7" 数字代表指定压缩等级（默认等级为8），数字必传
	 *		标准开源speex编码以及讯飞定制speex说明请参考音频格式说明
	 *
	 */
	UPROPERTY()
	FString aue = TEXT("raw");



	/**
	 * 参数名:sfl
	 * 类型:int
	 * 必传:否
	 * 描述:
	 *		需要配合aue=lame使用，开启流式返回
	 *		mp3格式音频
	 *		取值：1 开启
	 *
	 * 示例:1
	 * 本插件不使用该值
	 */
	 //UPROPERTY()
	int32 sfl = -1;

	/**
	 * 参数名:auf
	 * 类型:string
	 * 必传:否
	 * 描述:
	 *		音频采样率，可选值：
	 *		audio/L16;rate=8000：合成8K 的音频
	 *		audio/L16;rate=16000：合成16K 的音频
	 *		auf不传值：合成16K 的音频
	 * 示例:
	 *		"audio/L16;rate=16000"
	 *
	 */
	UPROPERTY()
	FString auf = TEXT("audio/L16;rate=16000");



	/**
	 * 参数名:vcn
	 * 类型:string
	 * 必传:是
	 * 描述:
	 *		发音人，可选值：请到控制台添加试用或购买发音人，添加后即显示发音人参数值
	 * 示例:
	 *		"xiaoyan"
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	FString vcn = TEXT("xiaoyan");

	/**
	 * 参数名:speed
	 * 类型:int
	 * 必传:否
	 * 描述:
	 *		语速，可选值：[0-100]，默认为50
	 * 示例:
	 *		50
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	int32 speed = 50;

	/**
	 * 参数名:volume
	 * 类型:int
	 * 必传:否
	 * 描述:
	 *		音量，可选值：[0-100]，默认为50
	 * 示例:
	 *		50
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	int32 volume = 50;

	/**
	 * 参数名:pitch
	 * 类型:int
	 * 必传:否
	 * 描述:
	 *		音量，可选值：[0-100]，默认为50
	 * 示例:
	 *		50
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	int32 pitch = 50;

	/**
	 * 参数名:bgs
	 * 类型:int
	 * 必传:否
	 * 描述:
	 *		合成音频的背景音
	 *		0:无背景音（默认值）
	 *		1:有背景音
	 * 示例:
	 *		0
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	int32 bgs = 0;


	/**
	 * 参数名:tte
	 * 类型:string
	 * 必传:否
	 * 描述:
	 *			文本编码格式
	 *			GB2312
	 *			GBK
	 *			BIG5
	 *			UNICODE(小语种必须使用UNICODE编码，合成的文本需使用utf16小端的编码方式，详见java示例demo)
	 *			GB18030
	 *			UTF8（小语种）
	 * 示例:
	 *		"UTF8"
	 *
	 */
	UPROPERTY()
	FString tte = TEXT("UTF8");

	/**
	 * 参数名:reg
	 * 类型:string
	 * 必传:否
	 * 描述:
	 *			设置英文发音方式：
	 *			0：自动判断处理，如果不确定将按照英文词语拼写处理（缺省）
	 *			1：所有英文按字母发音
	 *			2：自动判断处理，如果不确定将按照字母朗读
	 *			默认按英文单词发音
	 * 示例:
	 *		"2"
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	FString reg = TEXT("2");

	/**
	 * 参数名:rdn
	 * 类型:string
	 * 必传:否
	 * 描述:
	 *			合成音频数字发音方式
	 *			0：自动判断（默认值）
	 *			1：完全数值
	 *			2：完全字符串
	 *			3：字符串优先	"0"
	 * 示例:		"0"
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
	 * 参数名:text
	 * 类型:string
	 * 必传:是
	 * 描述:
	 *			文本内容，需进行base64编码；
	 *			base64编码前最大长度需小于8000字节，约2000汉字
	 *
	 */
	UPROPERTY()
	FString text = TEXT("");


	/**
	 * 参数名:text
	 * 类型:int
	 * 必传:是
	 * 描述:
	 *			数据状态，固定为2
	 *			注：由于流式合成的文本只能一次性传输，不支持多次分段传输，此处status必须为2。
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek TTS")
	FXGXunFeiTTSReqBusinessInfo business;

protected:

	UPROPERTY()
	FXGXunFeiTTSReqCommonInfo common;

	UPROPERTY()
	FXGXunFeiTTSReqDataInfo data;

};