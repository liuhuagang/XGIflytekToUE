// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"


#include "XGiFlyTekRealTimeSTTType.h"
#include "XGiFlyTekRealTimeSTTReqType.h"

#include "XGiFlyTekRealTimeSTTSubsystem.generated.h"



class FXGiFlyTekConsumeVoiceRunnable;
class IWebSocket;

UCLASS()
class  XGIFLYTEKSTT_API UXGiFlyTekRealTimeSTTSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class FXGiFlyTekConsumeVoiceRunnable;
	friend class UXGiFlyTekToUEBPLibrary;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;
	virtual void Deinitialize()override;


protected:

	void XGBeginRealTimeSpeechToText(
		FXGiFlyTekRealTimeSTTReqInfo& InRealTimeSTTReqInfo,
		FXGiFlyTekInitRealTimeSTTDelegate InInitRealTimeSTTDelegate,
		FXGiFlyTekRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate,
		FXGiFlyTekRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate);
	void XGStopRealTimeSpeechToText();



	 void SendVoiceData( const TArray<float>& InVoiceData);

	 void RealeaseVoiceConsumeRunnable();

	 void EndSendVoiceData();



protected:

	void OnConnected();
	void OnConnectionError(const FString& ErrorMessage);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);


protected:

	EXGiFlyTekRealTimeSTTStatus ReakTimeSTTStatus = EXGiFlyTekRealTimeSTTStatus::Ready;

	FXGiFlyTekInitRealTimeSTTDelegate InitRealTimeSTTDelegate;

	FXGiFlyTekRealTimeSTTTranslateDelegate RealTimeSTTTranslateDelegate;

	FXGiFlyTekRealTimeSTTNoTranslateDelegate RealTimeSTTNoTranslateDelegate;

	TSharedPtr<IWebSocket> Socket;
	
	FCriticalSection SocketCriticalSection;

	TSharedPtr<FXGiFlyTekConsumeVoiceRunnable> ConsumeVoiceRunnable;
};