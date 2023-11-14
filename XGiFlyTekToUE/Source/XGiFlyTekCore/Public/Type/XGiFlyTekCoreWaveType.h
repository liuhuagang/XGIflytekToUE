// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 头信息 格式+总数据大小
// 格式描述信息 关于数据格式的描述
// 数据信息描述 +数据大小
//  真正的数据
//12个字节
struct FXGiFlyTekWaveHeard
{
	//4个字节
	int8 ChunkID[4];
	//4个字节 36+数据大小 不包含 ChunkID  ChunkSize 4+ 24 +8 =36
	uint32 ChunkSize;
	//4个字节 
	int8 Format[4];
};

//24个字节
struct FXGiFlyTekWaveFmt
{
	//4个字节 
	int8 SubChunkID[4];
	//4个字节 16字节 不包含SubChunkID  SubChunkSize 2+2+4+4+2+2 =16
	uint32 SubChunkSize;
	//2个字节 1 
	uint16 AudioFormat;
	//2个字节 1
	uint16 NumChannel;
	//4个字节 16000
	uint32 SampleRate;
	//4个字节 byte
	uint32 ByteRate;
	//2个字节  2个字节对齐
	uint16 BlockAlign;
	//2个字节  16位
	uint16 BitsForSample;



};
//8个字节
struct FXGiFlyTekWaveData
{
	//4个字节
	int8 DataChunkID[4];
	//4个字节
	uint32 DataChunkSize;

};
//追加二进制数据