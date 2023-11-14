// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

#include "XGiFlyTekRealTimeSTTRespType.generated.h"


USTRUCT(BlueprintType)
struct FXGiFlyTekRealTImeSTTRespInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString code;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString sid;
};


USTRUCT(BlueprintType)
struct FXGiFlyTekRealTimeSTTTranslateData
{
	GENERATED_BODY()

	/**
	 * 业务标识字段，开启翻译功能后值为 trans
	 * 翻译功能标识
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString biz;

	/**
	 * 目标语种翻译文本结果
	 * 	与原始文本src对应
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString dst;
	/**
	 * 翻译结束标识
	 * 如果为 true，标识翻译结果已推送完成
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	bool isEnd;

	/**
	 * 转写结果序号
	 * 从0开始
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 segId;

	/**
	 * 送翻译的原始文本
	 * 音频对应的识别文本
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString src;

	/**
	 * 结果类型标识
	 * 0-最终结果；1-中间结果
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 type;


	/**
	 * 句子在整段语音中的开始时间，单位毫秒(ms)
	 * 中间结果的bg为准确值
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 bg;


	/**
	 * 句子在整段语音中的结束时间，单位毫秒(ms)
	 * 中间结果的ed为0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 ed;

};


USTRUCT(BlueprintType)
struct FXGiFlyTekRealTimeSTTNoTranslateCWData
{
	GENERATED_BODY()

	/**
	 * 词识别结果
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString w;

	/**
	 * 词标识
	 * n-普通词；s-顺滑词（语气词）；p-标点
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString wp;
};


USTRUCT(BlueprintType)
struct FXGiFlyTekRealTimeSTTNoTranslateWSData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	TArray<FXGiFlyTekRealTimeSTTNoTranslateCWData> cw;
	/**
	 * 词在本句中的开始时间，单位是帧，1帧=10ms
	 * 即词在整段语音中的开始时间为(bg+wb*10)ms
	 * 中间结果的 wb 为 0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 wb;

	/**
	 * 词在本句中的结束时间，单位是帧，1帧=10ms
	 * 即词在整段语音中的结束时间为(bg+we*10)ms
	 * 中间结果的 we 为 0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 we;
};



USTRUCT(BlueprintType)
struct FXGiFlyTekRealTimeSTTNoTranslateRTData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	TArray<FXGiFlyTekRealTimeSTTNoTranslateWSData> ws;
};





USTRUCT(BlueprintType)
struct FXGiFlyTekRealTimeSTTNoTranslateSTData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString bg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString ed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	TArray<FXGiFlyTekRealTimeSTTNoTranslateRTData> rt;


	/**
	 * 结果类型标识
	 * 0-最终结果；1-中间结果
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FString type;
};



USTRUCT(BlueprintType)
struct FXGiFlyTekRealTimeSTTNoTranslateCNData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FXGiFlyTekRealTimeSTTNoTranslateSTData st;
};


USTRUCT(BlueprintType)
struct FXGiFlyTekRealTimeSTTNoTranslateData
{
	GENERATED_BODY()

	/**
	 * 推测是区域代号
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	FXGiFlyTekRealTimeSTTNoTranslateCNData cn;


	/**
	 * 转写结果序号
	 * 从0开始
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XG iFylTek RealTime STT")
	int32 seg_id;
};
