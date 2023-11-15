// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


/**
 * FXGXunFeiWaveHeard
 * 
 */
struct FXGXunFeiWaveHeard
{

	int8 ChunkID[4];

	uint32 ChunkSize;

	int8 Format[4];
};

/**
 * FXGXunFeiWaveFmt
 *
 */
struct FXGXunFeiWaveFmt
{

	int8 SubChunkID[4];

	uint32 SubChunkSize;

	uint16 AudioFormat;

	uint16 NumChannel;
	
	uint32 SampleRate;

	uint32 ByteRate;

	uint16 BlockAlign;

	uint16 BitsForSample;



};

/**
 * FXGXunFeiWaveData
 *
 */
struct FXGXunFeiWaveData
{

	int8 DataChunkID[4];

	uint32 DataChunkSize;

};
