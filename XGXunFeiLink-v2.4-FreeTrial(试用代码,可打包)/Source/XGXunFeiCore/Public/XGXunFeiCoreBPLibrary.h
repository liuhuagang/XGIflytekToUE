// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once


#include "Kismet/BlueprintFunctionLibrary.h"
#include "XGXunFeiCoreBPLibrary.generated.h"


class USoundWave;
/**
 * UXGXunFeiCoreBPLibrary.
 * Exposed for OtherModule to use.
 * Highly customizable,Do not use it.
 */
UCLASS()
class XGXUNFEICORE_API UXGXunFeiCoreBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static	FString XunFeiTTSHMACSHA256(const FString& InAPPSecreet, const FString& InData);

	static void ConvertPCMToWave(const TArray<uint8>& InPCMData, TArray<uint8>& OutWaveData);

	static USoundWave* ImportPCMToSoundWave(const TArray<uint8>& InPCMData);

	static bool LoadPitcureFileToBinaryData(const FString& InAbsoluteFilePath,TArray<uint8>& OutImgBinaryData);

	static void AssembleAuthUrl(FString IniFlyTekURL, FString InAPISecret, FString InAPIKey, FString& OutAuthURL, FString& OutProtocol, bool bGet = true);

	static void AssembleAuthUrl(FString IniFlyTekURL, FString InAPISecret, FString InAPIKey, FString& OutAuthURL, FString& OutProtocol, TMap<FString, FString>& OutUpgradeHeaders);
	
	static FString URLEncode(FString InURL);
};
