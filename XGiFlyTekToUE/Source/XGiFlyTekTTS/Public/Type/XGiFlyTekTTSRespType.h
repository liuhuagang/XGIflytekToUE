// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "XGiFlyTekTTSRespType.generated.h"



USTRUCT()
struct XGIFLYTEKTTS_API FXGiFlyTekTTSRespInfoData
{
	GENERATED_BODY()

public:


	/**
	 * 参数名:audio
	 * 类型:string
	 * 描述:
	 *		合成后的音频片段，采用base64编码
	 *
	 */
	UPROPERTY()
	FString audio=TEXT("");

	/**
	 * 参数名:code
	 * 类型:int
	 * 描述:
	 *		当前音频流状态，1表示合成中，2表示合成结束
	 *
	 */
	UPROPERTY()
	int32 status = -1;

	/**
	 * 参数名:ced
	 * 类型:string
	 * 描述:
	 *		合成进度，指当前合成文本的字节数
	 *		注：请注意合成是以句为单位切割的，若文本只有一句话，则每次返回结果的ced是相同的。
	 *
	 */
	UPROPERTY()
	FString ced=TEXT("");
};





USTRUCT()
struct XGIFLYTEKTTS_API FXGiFlyTekTTSRespInfo
{
	GENERATED_BODY()

	friend class UXGiFlyTekTTSAsyncAction;

public:

	/**
	 * 参数名:code
	 * 类型:int
	 * 描述:
	 *		返回码，0表示成功，其它表示异常，详情请参考错误码。
	 *
	 */
	UPROPERTY()
	int32 code=-1;

	/**
	 * 参数名:message
	 * 类型:string
	 * 描述:
	 *		描述信息
	 *
	 */
	UPROPERTY()
	FString message=TEXT("");

	/**
	 * 参数名:data
	 * 类型:object
	 * 描述:
	 *		data可能返回为null，参考示例代码时，注意进行非空判断
	 *
	 */
	UPROPERTY()
	FXGiFlyTekTTSRespInfoData data;

	/**
	 * 参数名:sid
	 * 类型:string
	 * 描述:
	 *		本次会话的id，只在第一帧请求时返回
	 *
	 */
	UPROPERTY()
	FString sid;



};
