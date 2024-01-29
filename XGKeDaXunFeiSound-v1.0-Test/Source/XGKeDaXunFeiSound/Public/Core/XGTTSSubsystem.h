// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <IWebSocket.h>
#include "Thread/ConsumeSoundRunnable.h"
#include "XGKeDaXunFeiSoundBPLibrary.h"
#include "XGTTSSubsystem.generated.h"

// 头信息 格式+总数据大小
// 格式描述信息 关于数据格式的描述
// 数据信息描述 +数据大小
//  真正的数据
//12个字节
struct FXGWaveHeard
{
	//4个字节
	int8 ChunkID[4];
	//4个字节 36+数据大小 不包含 ChunkID  ChunkSize 4+ 24 +8 =36
	uint32 ChunkSize;
	//4个字节 
	int8 Format[4];
};

//24个字节
struct FXGWaveFmt
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
struct FXGWaveData
{
	//4个字节
	int8 DataChunkID[4];
	//4个字节
	uint32 DataChunkSize;

};
//追加二进制数据




/**
 *
 */
UCLASS()
class XGKEDAXUNFEISOUND_API UXGTTSSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{

	GENERATED_BODY()
public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;
	virtual void Deinitialize()override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;




public:
	UFUNCTION(BlueprintCallable)
		void CreateSocket();
	UFUNCTION(BlueprintCallable)
		void CloseSocket();

public:
	UFUNCTION(BlueprintCallable)
		void ConvertPCMToWav();

	UFUNCTION(BlueprintCallable, meta = (WorldContext = WorldContextObject))
		USoundWave* PlayWavAudioInScene(const UObject* WorldContextObject);


public:
	static TArray<float> FinalUEData;
	TArray<int16> OriginPCMData;
	TArray<uint8> FinalPCMData;
	// FString AudioStr= TEXT("");
	void SampleRateConvert(float CurrentSR, float TargetSR, int32 NumChannels, const TArray<int16>& CurrentRecordedPCMData, int32 NumSamplesToConvert, TArray<int16>& OutConverted);
protected:

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);

protected:
	TSharedPtr<IWebSocket> Socket;
	FString ServerURL = TEXT("");
	FString ServerProtocol = TEXT("");


protected:

	FString HMACSHA256(FString APPSecreet, FString Data);
};
