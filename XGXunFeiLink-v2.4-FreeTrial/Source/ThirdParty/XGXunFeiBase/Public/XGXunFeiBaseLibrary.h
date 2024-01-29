// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

/**
 * XGXunFeiBaseSHA256
 * Save the result after HmacSha256 encryption
 */
struct XGXUNFEIBASE_API FXGXunFeiBaseSHA256
{
	uint8 Digest[32];

};
/**
 * FXGXunFeiBaseLibrary
 * Exposed to XGXunFeiCore to use encryption method
 */
class XGXUNFEIBASE_API FXGXunFeiBaseLibrary
{
public:

	/**
	 * HmacSha256
	 * 
	 * @param Input		Input What you want to encrypt data ptr.
	 * @param InputLen	The lenth of data.
	 * @param Key		The Key ptr  which you want to encrypt data with.
	 * @param KeyLen	The lenth of the key.
	 * @return FXGXunFeiBaseSHA256	Encryption Resulte
	 */
	static FXGXunFeiBaseSHA256 HmacSha256(const uint8* Input, size_t InputLen, const uint8* Key, size_t KeyLen);


};
