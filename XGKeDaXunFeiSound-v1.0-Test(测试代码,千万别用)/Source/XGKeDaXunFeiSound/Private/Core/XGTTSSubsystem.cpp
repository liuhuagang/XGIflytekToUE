// Copyright 2023 Xiao Gang. All Rights Reserved.


#include "XGTTSSubsystem.h"
#include "WebSocketsModule.h"
#include "XGKeDaXunFeiSoundSettings.h"
#include "XGAudioCaptureSubsystem.h"
#include "hmacsha256.h" 
/*#include "AudioRecordingManager.h"*/



TArray<float>  UXGTTSSubsystem::FinalUEData ={};
bool UXGTTSSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGTTSSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);




}

void UXGTTSSubsystem::Deinitialize()
{

}

void UXGTTSSubsystem::Tick(float DeltaTime)
{

}

bool UXGTTSSubsystem::IsTickable() const
{
	 return !IsTemplate(); 
}

TStatId UXGTTSSubsystem::GetStatId() const
{
 RETURN_QUICK_DECLARE_CYCLE_STAT(UXGTTSSubsystem, STATGROUP_Tickables);
}





void UXGTTSSubsystem::CreateSocket()
{
	FString APPID = UXGKeDaXunFeiSoundSettings::GetDataBaseSettings()->AppID;
	FString APIKey = UXGKeDaXunFeiSoundSettings::GetDataBaseSettings()->APIKeyTTSStream;
	FString APISecret = UXGKeDaXunFeiSoundSettings::GetDataBaseSettings()->APISecretTTSStream;


	//wss://tts-api.xfyun.cn/v2/tts?authorization=aG1hYyB1c2VybmFtZT0iZGE0ZjMyOWUyZmQwMGQ1NjE4NjVjNjRkZjU3NDNiMjAiLCBhbGdvcml0aG09ImhtYWMtc2hhMjU2IiwgaGVhZGVycz0iaG9zdCBkYXRlIHJlcXVlc3QtbGluZSIsIHNpZ25hdHVyZT0ic1RtbzRobDBMdmRLWTRLRjltcGJKV0htRFFzNC8xZ2ZPdUgwZnBZbVdnbz0i&date=Thu%2C%2001%20Aug%202019%2001%3A53%3A21%20GMT&host=tts-api.xfyun.cn
	FString Date = FDateTime::Now().UtcNow().ToHttpDate();
	//Thu, 01 Aug 2019 01:53:21 GMT
	ServerURL =TEXT("");
	

	FString host = TEXT("host: tts-api.xfyun.cn\n");
	FString date = TEXT("date: ") + Date + TEXT("\n");
	FString requireLine = TEXT("GET /v2/tts HTTP/1.1");

	FString signature_origin = host + date + requireLine;

	FString	signature = HMACSHA256(APISecret, signature_origin);
	//拿到签字了



	//api_key = "$api_key", algorithm = "hmac-sha256", headers = "host date request-line", signature = "$signature"


	FString authorization_origin = TEXT("api_key=");

	authorization_origin += TEXT("\"");
	authorization_origin += APIKey;
	authorization_origin += TEXT("\"");
	authorization_origin += TEXT(",algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=");
	authorization_origin += TEXT("\"");
	authorization_origin += signature;
	authorization_origin += TEXT("\"");


	FString authorization = FBase64::Encode(authorization_origin);


	Date.ReplaceInline(TEXT(","), TEXT("%2C"));
	Date.ReplaceInline(TEXT(" "), TEXT("%20"));
	Date.ReplaceInline(TEXT(":"), TEXT("%3A"));


	ServerURL = TEXT("ws://tts-api.xfyun.cn/v2/tts?authorization=");
	ServerURL += authorization;
	ServerURL += TEXT("&date=");
	ServerURL += Date;
	ServerURL += TEXT("&host=tts-api.xfyun.cn");




	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL);
	Socket->OnConnected().AddUObject(this, &UXGTTSSubsystem::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UXGTTSSubsystem::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UXGTTSSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UXGTTSSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UXGTTSSubsystem::OnMessageSent);

	Socket->Connect();


}

void UXGTTSSubsystem::CloseSocket()
{	


}

void UXGTTSSubsystem::ConvertPCMToWav()
{
	TArray<uint8> OutWavData;


	FXGWaveHeard WaveHeard;
	FXGWaveFmt WaveFmt;
	FXGWaveData WaveData;

	//头描述
	FMemory::Memcpy(WaveHeard.ChunkID, "RIFF", strlen("RIFF"));
	FMemory::Memcpy(WaveHeard.Format, "WAVE", strlen("WAVE"));
	WaveHeard.ChunkSize = 36 + FinalPCMData.Num() * sizeof(uint8);

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
	WaveData.DataChunkSize = FinalPCMData.Num() * sizeof(uint8);

	//组合WAV格式数据
	//头 
	OutWavData.AddUninitialized(sizeof(FXGWaveHeard));
	FMemory::Memcpy(OutWavData.GetData(), &WaveHeard, sizeof(FXGWaveHeard));

	//格式描述
	int32 Index = OutWavData.AddUninitialized(sizeof(FXGWaveFmt));
	FMemory::Memcpy(&OutWavData[Index], &WaveFmt, sizeof(FXGWaveFmt));

	//数据
	Index = OutWavData.AddUninitialized(sizeof(FXGWaveData));
	FMemory::Memcpy(&OutWavData[Index], &WaveData, sizeof(FXGWaveData));

	//合并PCM
	OutWavData.Append(FinalPCMData);




	FString FilePah= FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir())/TEXT("tts.wav");
	FFileHelper::SaveArrayToFile(OutWavData, *FilePah);




}

USoundWave* UXGTTSSubsystem::PlayWavAudioInScene(const UObject* WorldContextObject)
{
	if (!WorldContextObject|| !WorldContextObject->GetWorld())
	{
		return nullptr;
	}

	UWorld* GameWorld = WorldContextObject->GetWorld();


	return nullptr;
}

void UXGTTSSubsystem::OnConnected()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));

	UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------------"));
	//FString MyText = TEXT("你好,小刚，我们成功实现了TTS文本转语音功能呢");

	TArray<uint8> OutArray;
	FString MyText = TEXT("你好，小刚，我们成功实现了文本转语音功能呢!");

	char* CharPtr = TCHAR_TO_UTF8(*MyText);
	int32 Length = strlen(CharPtr);



	FString Base64MyText = FBase64::Encode((uint8*)CharPtr, Length);



	// Convert FString to TCHAR array
	//const TCHAR* CharPtr = *MyText;
	//TArray<TCHAR> CharArray;
	//while (*CharPtr)
	//{
	//	CharArray.Add(*CharPtr++);
	//}

	// Convert TCHAR array to UTF-8 char array
	//FTCHARToUTF8 Converter(CharArray.GetData());
	//const auto ConvertedLength = Converter.Length();
	//OutArray.Reserve(ConvertedLength);
	//for (int32 i = 0; i < ConvertedLength; ++i)
	//{

	//	OutArray.Add(static_cast<uint8>(Converter.Get()[i]));
	//}



	//FString Base64MyText = FBase64::Encode(OutArray.GetData(), OutArray.Num());

	FString Json1 = TEXT("{\"common\": {\"app_id\": \"0654b4fb\"},\"business\" : {\"aue\": \"raw\",\"vcn\" : \"xiaoyan\",\"pitch\" : 50,\"speed\" : 50,\"tte\":\"UTF8\"},	\"data\": {\"status\": 2,\"text\" : ");
	Json1 += TEXT("\"");
	Json1 += Base64MyText;
	Json1 += TEXT("\"");
	Json1 += TEXT(" }} ");




	Socket->Send(Json1);


}




void UXGTTSSubsystem::OnConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Error:%s"), *FString(__FUNCTION__), *Error);


}

void UXGTTSSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp, Warning, TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean);

}

void UXGTTSSubsystem::OnMessage(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------------"));
	UE_LOG(LogTemp, Warning, TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);
	UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------------"));

	//string转json
	TSharedPtr<FJsonObject> MessageObject = MakeShareable(new FJsonObject);
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(Message);
	FJsonSerializer::Deserialize(Reader, MessageObject);

	//如果我们自己定义了结构体可以直接序列化成结构体


	int32 Code = MessageObject->GetIntegerField(TEXT("code"));

	if (Code == 0)
	{
		TSharedPtr<FJsonObject> DataObject = MessageObject->GetObjectField("data");

		FString Audio = DataObject->GetStringField(TEXT("audio"));
		int32	Status = DataObject->GetIntegerField(TEXT("status"));
		FString Ced = DataObject->GetStringField(TEXT("ced"));

	//	AudioStr.Append(Audio);
		//音频的二进制数据
		TArray<uint8> AudioData;

		//接收到的音频字符串 加密后的
		//解密，然后存到二进制数据组里面
		FBase64::Decode(Audio, AudioData);
	
		FinalPCMData.Append(AudioData);


		//PCM数据 16位 单通道 16k
		TArray<int16> PCMData;


		//01  23  34 

		for (int index = 0; index < AudioData.Num(); index += 2)
		{
			int16 a;

			int16 b;

			int16 PCMint16;
			//0100 0010
			a = AudioData[index + 1];
			//0010 0100 
			b = AudioData[index];
			//小端存储噢~~~

			// 0100 0010 0010 0100
			PCMint16 = (a << 8) | b; //a放在高8位，b放在低8位

			PCMData.Add(PCMint16);

		}

		OriginPCMData.Append(PCMData);
;
		TArray<float> UEData;

		for (auto& Tmp : PCMData)
		{
			UEData.Add(FMath::Clamp(Tmp / 32767.f, -1.0, 1.0));
		}

		FinalUEData.Append(UEData);

	}




	

}

void UXGTTSSubsystem::OnMessageSent(const FString& MessageString)
{
	UE_LOG(LogTemp, Warning, TEXT("%s MessageString:%s"), *FString(__FUNCTION__), *MessageString);
}

FString UXGTTSSubsystem::HMACSHA256(FString APPSecreet, FString Data)
{


	uint8_t md[SHA256_DIGESTLEN] = { 0 };
	uint8_t* secret = (uint8_t*)TCHAR_TO_UTF8(*APPSecreet);
	uint8_t* indata = (uint8_t*)TCHAR_TO_UTF8(*Data);

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
 void UXGTTSSubsystem::SampleRateConvert(float CurrentSR, float TargetSR, int32 NumChannels, const TArray<int16>& CurrentRecordedPCMData, int32 NumSamplesToConvert, TArray<int16>& OutConverted)
{
	int32 NumInputSamples = CurrentRecordedPCMData.Num();
	int32 NumOutputSamples = NumInputSamples * TargetSR / CurrentSR;

	OutConverted.Reset(NumOutputSamples);

	float SrFactor = (double)CurrentSR / TargetSR;
	float CurrentFrameIndexInterpolated = 0.0f;
	check(NumSamplesToConvert <= CurrentRecordedPCMData.Num());
	int32 NumFramesToConvert = NumSamplesToConvert / NumChannels;
	int32 CurrentFrameIndex = 0;

	for (;;)
	{
		int32 NextFrameIndex = CurrentFrameIndex + 1;
		if (CurrentFrameIndex >= NumFramesToConvert || NextFrameIndex >= NumFramesToConvert)
		{
			break;
		}

		for (int32 Channel = 0; Channel < NumChannels; ++Channel)
		{
			int32 CurrentSampleIndex = CurrentFrameIndex * NumChannels + Channel;
			int32 NextSampleIndex = CurrentSampleIndex + NumChannels;

			int16 CurrentSampleValue = CurrentRecordedPCMData[CurrentSampleIndex];
			int16 NextSampleValue = CurrentRecordedPCMData[NextSampleIndex];

			int16 NewSampleValue = FMath::Lerp(CurrentSampleValue, NextSampleValue, CurrentFrameIndexInterpolated);

			OutConverted.Add(NewSampleValue);
		}

		CurrentFrameIndexInterpolated += SrFactor;

		// Wrap the interpolated frame between 0.0 and 1.0 to maintain float precision
		while (CurrentFrameIndexInterpolated >= 1.0f)
		{
			CurrentFrameIndexInterpolated -= 1.0f;

			// Every time it wraps, we increment the frame index
			++CurrentFrameIndex;
		}
	}
}