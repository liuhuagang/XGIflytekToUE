// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "XGXunFeiTTSRespType.generated.h"



USTRUCT()
struct XGXUNFEITTS_API FXGXunFeiTTSRespInfoData
{
	GENERATED_BODY()

public:


	/**
	 * Parameter:audio
	 * Type:string
	 * Description:
	 *		The synthesized audio snippets are encoded in base64
	 *
	 */
	UPROPERTY()
	FString audio=TEXT("");

	/**
	 * Parameter:code
	 * Type:int
	 * Description:
	 *		Current audio stream status, 1 indicates synthesis, 2 indicates synthesis end
	 *
	 */
	UPROPERTY()
	int32 status = -1;

	/**
	 * Parameter:ced
	 * Type:string
	 * Description:
	 *		Synthesis progress refers to the number of bytes of the current synthesized text
	 *		Note: Please note that the composition is cut by sentence, if there is only one sentence of text, the ced is the same each time the result is returned.
	 *
	 */
	UPROPERTY()
	FString ced=TEXT("");
};





USTRUCT()
struct XGXUNFEITTS_API FXGXunFeiTTSRespInfo
{
	GENERATED_BODY()

	friend class UXGXunFeiTTSAsyncAction;

public:

	/**
	 * Parameter:code
	 * Type:int
	 * Description:
	 *		Return code, 0 indicates success, other indicates exception. For details, see the error code.
	 *
	 */
	UPROPERTY()
	int32 code=-1;

	/**
	 * Parameter:message
	 * Type:string
	 * Description:
	 *		Description
	 *
	 */
	UPROPERTY()
	FString message=TEXT("");

	/**
	 * Parameter名:data
	 * Type:object
	 * Description:
	 *		data may be returned as null. When referring to the Example code, be careful to make a non-null judgment
	 *
	 */
	UPROPERTY()
	FXGXunFeiTTSRespInfoData data;

	/**
	 * Parameter名:sid
	 * Type:string
	 * Description:
	 *		The id of this session is returned only on the first frame request
	 *
	 */
	UPROPERTY()
	FString sid;



};
