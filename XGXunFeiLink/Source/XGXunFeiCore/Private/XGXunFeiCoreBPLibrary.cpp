// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiCoreBPLibrary.h"

#include "Sound/SoundWaveProcedural.h"
#include "Misc/Base64.h"

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
	//TODO ModifyAPi to Stream~~
	//if (false)
	//{
	//	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());

	//	SoundWave->SetSampleRate(16000);
	//	SoundWave->SetImportedSampleRate(16000);
	//	SoundWave->NumChannels = 1;
	//	SoundWave->SampleByteSize = 16;
	//	SoundWave->bLooping = false;
	//	SoundWave->bCanProcessAsync = true;
	//	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	//	SoundWave->QueueAudio(InPCMData.GetData(), InPCMData.Num());
	//}

	USoundWave* NewSoundWave = NewObject<USoundWave>();

	uint8* RawData = nullptr;

	RawData = (uint8*)FMemory::Malloc(InPCMData.Num());
	FMemory::Memcpy(RawData, InPCMData.GetData(), InPCMData.Num());

	int32 NumBytes = 0;
	int32 NumFrames = InPCMData.Num() / 2;

	NumBytes = NumFrames * sizeof(int16);

	NewSoundWave->RawPCMDataSize = NumBytes;
	NewSoundWave->RawPCMData = RawData;
	NewSoundWave->Duration = (float)NumFrames / 16000.f;
	NewSoundWave->SetSampleRate(16000);
	NewSoundWave->NumChannels = 1;
	NewSoundWave->bLooping=false;

	NewSoundWave->SoundWaveDataPtr->InitializeDataFromSoundWave(*NewSoundWave);

	return NewSoundWave;
}
