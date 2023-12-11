// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiBaseLibrary.h"

#if WITH_SSL
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/ssl.h>
#endif

FXGXunFeiBaseSHA256 FXGXunFeiBaseLibrary::HmacSha256(const uint8* Input, size_t InputLen, const uint8* Key, size_t KeyLen)
{
	FXGXunFeiBaseSHA256 Output;
	unsigned int OutputLen = 0;
	HMAC(EVP_sha256(), Key, KeyLen, (const unsigned char*)Input, InputLen, Output.Digest, &OutputLen);
	return Output;

}
