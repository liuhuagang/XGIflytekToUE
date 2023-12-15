// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiRealTimeSTTReqType.h"

#include "Misc/Base64.h"

#include "XGXunFeiCoreSettings.h"



FString FXGXunFeiRealTimeSTTReqInfo::GenerateRequireParams()
{
	FString RetStr=TEXT("");

	FString AppID = UXGXunFeiCoreSettings::GetXGXunFeiCoreSettings()->App_ID_RealTimeSTT;
	FString APIKey = UXGXunFeiCoreSettings::GetXGXunFeiCoreSettings()->API_Key_RealTimeSTT;
	FString TS = FString::FromInt(FDateTime::Now().UtcNow().ToUnixTimestamp());

	appid = AppID;

	ts = TS;

	signa = GenerateSigna(AppID, TS, APIKey);

	RetStr += TEXT("appid=") + appid;
	RetStr += TEXT("&ts=") + ts;
	RetStr += TEXT("&signa=") + signa;

	if (!lang.IsEmpty())
	{
		RetStr += TEXT("&lang=") + lang;
	}

	if (bEnableTranslate)
	{
		RetStr += TEXT("&transType=") + transType;
		RetStr += TEXT("&transStrategy=") + FString::FromInt(transStrategy);
		RetStr += TEXT("&targetLang=") + targetLang;
	}

	if (!punc.IsEmpty()&&!punc.Equals(TEXT("None")))
	{
		RetStr+= TEXT("&punc=") + punc;
	}


	if (!pd.IsEmpty()&& !pd.Equals(TEXT("None")))
	{
		RetStr += TEXT("&pd=") + pd;
	}

	if (vadMdn!=-1)
	{
		RetStr += TEXT("&vadMdn=") + FString::FromInt(vadMdn);
	}


	if (roleType != -1)
	{
		RetStr += TEXT("&roleType=") + FString::FromInt(roleType);
	}



	if (engLangType != -1)
	{
		RetStr += TEXT("&engLangType=") + FString::FromInt(engLangType);
	}

	return RetStr;
}

FString FXGXunFeiRealTimeSTTReqInfo::GenerateSigna(const FString& InAppid, const FString& InTS, const FString& InAPIKey)
{
	FString BaseString = InAppid+ InTS;
	FString MD5BaseString = FMD5::HashAnsiString(*BaseString);

	FSHAHash Hash;
	FSHA1 SHA1;
	SHA1.HMACBuffer(TCHAR_TO_ANSI(*InAPIKey), InAPIKey.Len(), TCHAR_TO_ANSI(*MD5BaseString), MD5BaseString.Len(), Hash.Hash);

	FString Signa = FBase64::Encode(Hash.Hash, 20);

	Signa.ReplaceInline(TEXT("%"), TEXT("%25"));
	Signa.ReplaceInline(TEXT("+"), TEXT("%2B"));
	Signa.ReplaceInline(TEXT(" "), TEXT("%20"));
	Signa.ReplaceInline(TEXT("/"), TEXT("%2F"));
	Signa.ReplaceInline(TEXT("?"), TEXT("%3F"));
	Signa.ReplaceInline(TEXT("#"), TEXT("%23"));
	Signa.ReplaceInline(TEXT("&"), TEXT("%26"));
	Signa.ReplaceInline(TEXT("="), TEXT("%3D"));

	return Signa;
}

