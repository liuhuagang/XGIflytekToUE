// Copyright Xiao Gang. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "XGiFlyTekCoreBPLibrary.generated.h"

class USoundWave;
UCLASS()
class XGIFLYTEKCORE_API UXGiFlyTekCoreBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static	FString iFlyTekTTSHMACSHA256(const FString& InAPPSecreet, const FString& InData);

	static void ConvertPCMToWave(const TArray<uint8>& InPCMData, TArray<uint8>& OutWaveData);

	static USoundWave* ImportPCMToSoundWave(const TArray<uint8>& InPCMData);
};
