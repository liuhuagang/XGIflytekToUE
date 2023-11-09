// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGRealTimeSpeechToTextRequestType.h"
#include "XGKeDaXunFeiSoundSettings.h"

FString FXGRealTimeSpeechToTextRequestType::GenerateRequireParams()
{
	FString RetStr=TEXT("");

	FString AppID = UXGKeDaXunFeiSoundSettings::GetDataBaseSettings()->AppID;
	FString APIKey = UXGKeDaXunFeiSoundSettings::GetDataBaseSettings()->APIKeySTT;
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

FString FXGRealTimeSpeechToTextRequestType::GenerateSigna(const FString& InAppid, const FString& InTS, const FString& InAPIKey)
{
	FString BaseString = InAppid+ InTS;
	FString MD5BaseString = FMD5::HashAnsiString(*BaseString);

	FSHAHash Hash;
	FSHA1 SHA1;
	SHA1.HMACBuffer(TCHAR_TO_ANSI(*InAPIKey), InAPIKey.Len(), TCHAR_TO_ANSI(*MD5BaseString), MD5BaseString.Len(), Hash.Hash);

	FString Signa = FBase64::Encode(Hash.Hash, 20);
	Signa.ReplaceInline(TEXT("="), TEXT("%3D"));

	return Signa;
}

