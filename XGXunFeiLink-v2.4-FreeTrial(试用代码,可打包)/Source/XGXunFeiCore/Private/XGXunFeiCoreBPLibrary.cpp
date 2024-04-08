// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiCoreBPLibrary.h"

#include "Sound/SoundWaveProcedural.h"
#include "Misc/Base64.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Runtime/Launch/Resources/Version.h"
#include "UObject/Package.h"
#include "UObject/ObjectMacros.h"
#include "Memory/SharedBuffer.h"




#include "XGXunFeiBaseLibrary.h"
#include "XGXunFeiCoreWaveType.h"

FString UXGXunFeiCoreBPLibrary::XunFeiTTSHMACSHA256(const FString& InAPPSecreet, const FString& InData)
{

	//uint8_t* secret = (uint8_t*)TCHAR_TO_UTF8(*InAPPSecreet);
	//uint8_t* indata = (uint8_t*)TCHAR_TO_UTF8(*InData);

	int len1 = strlen((char*)TCHAR_TO_UTF8(*InAPPSecreet));
	int len2 = strlen((char*)TCHAR_TO_UTF8(*InData));

	FXGXunFeiBaseSHA256 XunFeiBaseSHA256 = FXGXunFeiBaseLibrary::HmacSha256((uint8_t*)TCHAR_TO_UTF8(*InData), len2, (uint8_t*)TCHAR_TO_UTF8(*InAPPSecreet), len1);

	FString RetStr = TEXT("");

	RetStr = FBase64::Encode(XunFeiBaseSHA256.Digest, 32);

	return RetStr;

}

void UXGXunFeiCoreBPLibrary::ConvertPCMToWave(const TArray<uint8>& InPCMData, TArray<uint8>& OutWaveData)
{
	OutWaveData.Empty();
	FXGXunFeiWaveHeard WaveHeard;
	FXGXunFeiWaveFmt WaveFmt;
	FXGXunFeiWaveData WaveData;

	FMemory::Memcpy(WaveHeard.ChunkID, "RIFF", strlen("RIFF"));
	FMemory::Memcpy(WaveHeard.Format, "WAVE", strlen("WAVE"));
	WaveHeard.ChunkSize = 36 + InPCMData.Num() * sizeof(uint8);

	FMemory::Memcpy(WaveFmt.SubChunkID, "fmt ", strlen("fmt "));
	WaveFmt.SubChunkSize = 16;
	WaveFmt.AudioFormat = 1;
	WaveFmt.NumChannel = 1;
	WaveFmt.SampleRate = 16000;
	WaveFmt.BitsForSample = 16;
	WaveFmt.ByteRate = 16000 * 1 * 16 / 8;
	WaveFmt.BlockAlign = 1 * 16 / 8;

	FMemory::Memcpy(WaveData.DataChunkID, "data", strlen("data"));
	WaveData.DataChunkSize = InPCMData.Num() * sizeof(uint8);


	OutWaveData.AddUninitialized(sizeof(FXGXunFeiWaveHeard));
	FMemory::Memcpy(OutWaveData.GetData(), &WaveHeard, sizeof(FXGXunFeiWaveHeard));


	int32 Index = OutWaveData.AddUninitialized(sizeof(FXGXunFeiWaveFmt));
	FMemory::Memcpy(&OutWaveData[Index], &WaveFmt, sizeof(FXGXunFeiWaveFmt));


	Index = OutWaveData.AddUninitialized(sizeof(FXGXunFeiWaveData));
	FMemory::Memcpy(&OutWaveData[Index], &WaveData, sizeof(FXGXunFeiWaveData));


	OutWaveData.Append(InPCMData);



}

USoundWave* UXGXunFeiCoreBPLibrary::ImportPCMToSoundWave(const TArray<uint8>& InPCMData)
{
	const int32 ChannelCount = 1;

	const int32 SizeOfSample = 16 / 8;
	const int32 NumSamples = InPCMData.Num() / SizeOfSample;

	const int32 NumFrames = NumSamples / ChannelCount;

	const int32 SampleDataSize = InPCMData.Num();

	FString SoundWaveName = TEXT("XGSoundWave:") + FGuid::NewGuid().ToString();

	USoundWave* SoundWave = NewObject < USoundWave>(GetTransientPackage(), *SoundWaveName, RF_Public | RF_Standalone);



#if WITH_EDITOR&& ENGINE_MINOR_VERSION > 0

	TArray<uint8> WavData;
	ConvertPCMToWave(InPCMData, WavData);

	SoundWave->RawData.UpdatePayload(FSharedBuffer::Clone(WavData.GetData(), WavData.Num()));
#endif  


	SoundWave->RawPCMData = (uint8*)FMemory::Malloc(SampleDataSize);

	FMemory::Memcpy(SoundWave->RawPCMData, InPCMData.GetData(), SampleDataSize);

	SoundWave->RawPCMDataSize = SampleDataSize;

	// Set Sound Wave Info
	SoundWave->Duration = (float)NumFrames / 16000.f;
	SoundWave->SetSampleRate(16000.f);
	SoundWave->NumChannels = ChannelCount;
	SoundWave->TotalSamples = NumFrames;




	return SoundWave;
}

bool UXGXunFeiCoreBPLibrary::LoadPitcureFileToBinaryData(const FString& InAbsoluteFilePath, TArray<uint8>& OutImgBinaryData)
{
	if (FPaths::FileExists(InAbsoluteFilePath))
	{
		OutImgBinaryData.Empty();
		return  FFileHelper::LoadFileToArray(OutImgBinaryData, *InAbsoluteFilePath);
	}



	return false;
}
