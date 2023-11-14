// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

#include "XGiFlyTekRealTimeSTTReqType.generated.h"


/**
 * 科大讯飞实时语音转写官方文档
 * https://www.xfyun.cn/doc/asr/rtasr/API.html#%E6%8E%A5%E5%8F%A3%E8%B0%83%E7%94%A8%E6%B5%81%E7%A8%8B
 *
 */
USTRUCT(BlueprintType)
struct XGIFLYTEKSTT_API FXGiFlyTekRealTimeSTTReqInfo
{
	GENERATED_BODY()

protected:

	/**
	 * 参数:appid
	 * 类型:string
	 * 必须:是
	 * 说明:讯飞开放平台应用ID
	 * 示例:5f9c6d6a
	 */
	UPROPERTY()
	FString appid;

	/**
	 * 参数:ts
	 * 类型:string
	 * 必须:是
	 * 说明:当前时间戳，从1970年1月1日0点0分0秒开始到现在的秒数
	 * 示例:1512041814
	 */
	UPROPERTY()
	FString ts;

	/**
	 * 参数:signa
	 * 类型:string
	 * 必须:是
	 * 说明:加密数字签名（基于HMACSHA1算法）
	 * 示例:IrrzsJeOFk1NGfJHW6SkHUoN9CU=
	 */
	UPROPERTY()
	FString signa;

public:

	/**
	 * 参数:lang
	 * 类型:string
	 * 必须:否
	 * 说明:实时语音转写语种，不传默认为中文
	 * 示例:
	 *			语种类型：中文、中英混合识别：cn；
	 *			英文：en；
	 *			小语种及方言可到控制台-实时语音转写-方言/语种处添加，添加后会显示该方言/语种参数值。传参示例如："lang=en"
	 * 若未授权无法使用会报错10110
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString lang = TEXT("cn");

	/**
	 * 自定义字段 是否开启翻译功能
	 * 需要在科大讯飞控制台开通翻译功能
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	bool bEnableTranslate = false;


	/**
	 * 参数:transType
	 * 类型:string
	 * 必须:否
	 * 说明:normal表示普通翻译，默认值normal；
	 * 示例:	例如：transType="normal"
	 * 注意：需控制台开通翻译功能
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString transType = TEXT("normal");

	/**
	 * 参数:transType
	 * 类型:int
	 * 必须:否
	 * 说明:	策略1，转写的vad结果直接送去翻译；
	 *		策略2，返回中间过程中的结果；
	 *		策略3，按照结束性标点拆分转写结果请求翻译；
	 *		建议使用策略2
	 * 示例:	例如：transStrategy=2
	 * 注意：需控制台开通翻译功能
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 transStrategy = 2;


	/**
	 * 参数:targetLang
	 * 类型:string
	 * 必须:否
	 * 说明:
	 *		目标翻译语种：控制把源语言转换成什么类型的语言；
	 *			请注意类似英文转成法语必须以中文为过渡语言，即英-中-法，暂不支持不含中文语种之间的直接转换；
	 *			中文：cn
	 *			英文：en
	 *			日语：ja
	 *			韩语：ko
	 *			俄语：ru
	 *			法语：fr
	 *			西班牙语：es
	 *			越南语：vi
	 *			广东话：cn_cantonese
	 * 示例:
	 *		例如：targetLang="en"
	 *		如果使用中文实时翻译为英文传参示例如下：
	 *		"&lang=cn&transType=normal&transStrategy=2&targetLang=en"
	 * 注意：需控制台开通翻译功能
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString targetLang = TEXT("en");

	/**
	 * 参数:punc
	 * 类型:string
	 * 必须:否
	 * 说明:	标点过滤控制，默认返回标点，punc=0会过滤结果中的标点
	 * 示例: 0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString punc = TEXT("None");

	/**
	 * 参数:pd
	 * 类型:string
	 * 必须:否
	 * 说明:
	 *		垂直领域个性化参数:
	 *		法院: court
	 *		教育: edu
	 *		金融: finance
	 *		医疗: medical
	 *		科技: tech
	 *		运营商: isp
	 *		政府: gov
	 *		电商: ecom
	 *		军事: mil
	 *		企业: com
	 *		生活: life
	 *		汽车: car
	 * 示例:
	 *		设置示例：pd="edu"
	 *		参数pd为非必须设置，不设置参数默认为通用
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString pd = TEXT("None");


	/**
	 * 参数:vadMdn
	 * 类型:int
	 * 必须:否
	 * 说明:	远近场切换，不传此参数或传1代表远场，传2代表近场
	 * 示例:	设置示例：vadMdn=2
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 vadMdn = -1;

	/**
	 * 参数:roleType
	 * 类型:int
	 * 必须:否
	 * 说明:	是否开角色分离，默认不开启，传2开启(效果持续优化中)
	 * 示例:	设置示例：roleType=2
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 roleType = -1;

	/**
	 * 参数:engLangType
	 * 类型:int
	 * 必须:否
	 * 说明:
	 *		语言识别模式选，默认为模式1中英文模式：
	 *		1：自动中英文模式
	 *		2：中文模式，可能包含少量英文
	 *		4：纯中文模式，不包含英文
	 * 示例:	设置示例：engLangType=4
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 engLangType = -1;

public:

	FString GenerateRequireParams();

protected:

	FString GenerateSigna(const FString& InAppid, const FString& InTS, const FString& InAPIKey);

};

