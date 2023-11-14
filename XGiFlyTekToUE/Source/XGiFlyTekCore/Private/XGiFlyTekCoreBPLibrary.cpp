// Copyright Xiao Gang. All Rights Reserved.

#include "XGiFlyTekCoreBPLibrary.h"
#include "XGiFlyTekCoreWaveType.h"
#include "hmacsha256.h"
#include "Sound/SoundWaveProcedural.h"
#include "Misc/Base64.h"

FString UXGiFlyTekCoreBPLibrary::iFlyTekTTSHMACSHA256(const FString& InAPPSecreet, const FString& InData)
{
	uint8_t md[SHA256_DIGESTLEN] = { 0 };
	uint8_t* secret = (uint8_t*)TCHAR_TO_UTF8(*InAPPSecreet);
	uint8_t* indata = (uint8_t*)TCHAR_TO_UTF8(*InData);

	int len1 = strlen((char*)secret);
	int len2 = strlen((char*)indata);

	HMAC_SHA256_CTX hmac;

	hmac_sha256_init(&hmac, secret, len1);
	hmac_sha256_update(&hmac, indata, len2);
	hmac_sha256_final(&hmac, md);

	FString RetStr = TEXT("");

	RetStr = FBase64::Encode(md, SHA256_DIGESTLEN);

	return RetStr;
}

void UXGiFlyTekCoreBPLibrary::ConvertPCMToWave(const TArray<uint8>& InPCMData, TArray<uint8>& OutWaveData)
{
	OutWaveData.Empty();
	FXGiFlyTekWaveHeard WaveHeard;
	FXGiFlyTekWaveFmt WaveFmt;
	FXGiFlyTekWaveData WaveData;

	//头描述
	FMemory::Memcpy(WaveHeard.ChunkID, "RIFF", strlen("RIFF"));
	FMemory::Memcpy(WaveHeard.Format, "WAVE", strlen("WAVE"));
	WaveHeard.ChunkSize = 36 + InPCMData.Num() * sizeof(uint8);

	//格式描述
	FMemory::Memcpy(WaveFmt.SubChunkID, "fmt ", strlen("fmt "));
	WaveFmt.SubChunkSize = 16;
	WaveFmt.AudioFormat = 1;
	WaveFmt.NumChannel = 1;
	WaveFmt.SampleRate = 16000;
	WaveFmt.BitsForSample = 16;
	WaveFmt.ByteRate = 16000 * 1 * 16 / 8;
	WaveFmt.BlockAlign = 1 * 16 / 8;

	//数据
	FMemory::Memcpy(WaveData.DataChunkID, "data", strlen("data"));
	WaveData.DataChunkSize = InPCMData.Num() * sizeof(uint8);

	//组合WAV格式数据
	//头 
	OutWaveData.AddUninitialized(sizeof(FXGiFlyTekWaveHeard));
	FMemory::Memcpy(OutWaveData.GetData(), &WaveHeard, sizeof(FXGiFlyTekWaveHeard));

	//格式描述
	int32 Index = OutWaveData.AddUninitialized(sizeof(FXGiFlyTekWaveFmt));
	FMemory::Memcpy(&OutWaveData[Index], &WaveFmt, sizeof(FXGiFlyTekWaveFmt));

	//数据
	Index = OutWaveData.AddUninitialized(sizeof(FXGiFlyTekWaveData));
	FMemory::Memcpy(&OutWaveData[Index], &WaveData, sizeof(FXGiFlyTekWaveData));

	//合并PCM
	OutWaveData.Append(InPCMData);



}

USoundWave* UXGiFlyTekCoreBPLibrary::ImportPCMToSoundWave(const TArray<uint8>& InPCMData)
{
	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());

	SoundWave->SetSampleRate(16000);
	SoundWave->SetImportedSampleRate(16000);
	SoundWave->NumChannels =1;
	SoundWave->SampleByteSize = 16;
	SoundWave->QueueAudio(InPCMData.GetData(), InPCMData.Num());

	return SoundWave;
}
