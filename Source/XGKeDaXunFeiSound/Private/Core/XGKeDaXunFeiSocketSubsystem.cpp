// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/XGKeDaXunFeiSocketSubsystem.h"
#include "WebSocketsModule.h"
#include "Type/XGAppInformation.h"

bool UXGKeDaXunFeiSocketSubsystem::bSending = false;
TSharedPtr<IWebSocket> UXGKeDaXunFeiSocketSubsystem::Socket = {};
bool UXGKeDaXunFeiSocketSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGKeDaXunFeiSocketSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Appid = FXGAppInformation::Appid;
	APIKey = FXGAppInformation::APIKey;



	FConsumeSoundRunnable* Runnable1 = new FConsumeSoundRunnable(TEXT("线程1"));
	FConsumeSoundRunnable* Runnable2 = new FConsumeSoundRunnable(TEXT("线程2"));
	FRunnableThread* RunnableThread1 = FRunnableThread::Create(Runnable1, *Runnable1->MyThreadName);
	FRunnableThread* RunnableThread2 = FRunnableThread::Create(Runnable2, *Runnable2->MyThreadName);
	Runnable2->Stop();

}

void UXGKeDaXunFeiSocketSubsystem::Deinitialize()
{

}

void UXGKeDaXunFeiSocketSubsystem::Tick(float DeltaTime)
{

}

bool UXGKeDaXunFeiSocketSubsystem::IsTickable() const
{
	 return !IsTemplate(); //不是CDO才Tick
}

TStatId UXGKeDaXunFeiSocketSubsystem::GetStatId() const
{
 RETURN_QUICK_DECLARE_CYCLE_STAT(UXGKeDaXunFeiSocketSubsystem, STATGROUP_Tickables); 
}

void UXGKeDaXunFeiSocketSubsystem::CreateSocket()
{

	FModuleManager::Get().LoadModuleChecked("WebSockets");


	FString BaseString = TEXT("");
	FString TS = TEXT("1512041814");
	TS = FString::FromInt(FDateTime::Now().UtcNow().ToUnixTimestamp());
	BaseString = Appid + TS;


	FString MD5BaseString = FMD5::HashAnsiString(*BaseString);


	FSHAHash Hash;
	FSHA1 SHA1;
	SHA1.HMACBuffer(TCHAR_TO_ANSI(*APIKey), APIKey.Len(), TCHAR_TO_ANSI(*MD5BaseString), MD5BaseString.Len(), Hash.Hash);
	FString WrongSigna1 = Hash.ToString();
	FString WrongSigna2 = FBase64::Encode(WrongSigna1);
	FString Signa = FBase64::Encode(Hash.Hash, 20);
	Signa.ReplaceInline(TEXT("="), TEXT("%3D"));

	ServerProtocol = TEXT("ws");
	//ws://rtasr.xfyun.cn/v1/ws?appid=595f23df&ts=1512041814&signa=IrrzsJeOFk1NGfJHW6SkHUoN9CU=&pd=edu
	ServerURL = TEXT("ws://rtasr.xfyun.cn/v1/ws?");
	ServerURL += TEXT("appid=");
	ServerURL += Appid;

	ServerURL += TEXT("&");
	ServerURL += TEXT("ts=");
	ServerURL += TS;

	ServerURL += TEXT("&");
	ServerURL += TEXT("signa=");
	ServerURL += Signa;


	ServerURL += TEXT("&");
	ServerURL += TEXT("pd=edu");



	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
	Socket->OnConnected().AddUObject(this, &UXGKeDaXunFeiSocketSubsystem::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UXGKeDaXunFeiSocketSubsystem::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UXGKeDaXunFeiSocketSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UXGKeDaXunFeiSocketSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UXGKeDaXunFeiSocketSubsystem::OnMessageSent);

	Socket->Connect();
	bSending = true;

}

void UXGKeDaXunFeiSocketSubsystem::CloseSocket()
{	
	bSending = false;
	if (Socket.IsValid()&& Socket->IsConnected())
	{
		Socket->Close();
	}


}

void UXGKeDaXunFeiSocketSubsystem::SendVoiceData(const float* InAudio, int32 NumSamples)
{
	if (Socket.IsValid()&& Socket->IsConnected()&& bSending)
	{
		//我们拿到的数据实际上2048个采样点 每个采样点都是float类型,4个字节,32位

		//现在每64ms 发送1024个采样点信息,一个采样点是2个字节,16位.
		//发送2048个字节

		TArray<int16> ToChangeAuidoData;
		TArray<uint8> BinaryDataToSend;
	//	int32 j = 0;
		int32 i = 0;
		for (; i < 1024;)
		{	//-1.0~1.0
			ToChangeAuidoData.Add((int16)FMath::Clamp<int32>(FMath::FloorToInt(32767.0f * InAudio[i]), -32768, 32767));

			uint8 Bytes[2];
			//取出数据的高位数据
			Bytes[0] = (uint8)(ToChangeAuidoData[i] & 0xFF);
			//取出数据的低位数据
			Bytes[1] = (uint8)((ToChangeAuidoData[i] >> 8) & 0xFF);


			//大端存储把高位数据放到低字节
			BinaryDataToSend.Add(Bytes[0]);
			//大端存储把地位数据放到高字节
			BinaryDataToSend.Add(Bytes[1]);

			i++;
	//		j = j + 2;

		}

		Socket->Send(BinaryDataToSend.GetData(), BinaryDataToSend.Num(),false);




	}



}

void UXGKeDaXunFeiSocketSubsystem::StopSenVoiceData()
{
	bSending = false;
}

void UXGKeDaXunFeiSocketSubsystem::OnConnected()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
}

void UXGKeDaXunFeiSocketSubsystem::OnConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Error:%s"), *FString(__FUNCTION__), *Error);
}

void UXGKeDaXunFeiSocketSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp, Warning, TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean);
	bSending = false;
}

void UXGKeDaXunFeiSocketSubsystem::OnMessage(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);
}

void UXGKeDaXunFeiSocketSubsystem::OnMessageSent(const FString& MessageString)
{
	UE_LOG(LogTemp, Warning, TEXT("%s MessageString:%s"), *FString(__FUNCTION__), *MessageString);
}
