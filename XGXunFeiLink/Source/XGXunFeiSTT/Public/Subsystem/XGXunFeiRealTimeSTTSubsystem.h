// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"


#include "XGXunFeiRealTimeSTTType.h"
#include "XGXunFeiRealTimeSTTReqType.h"

#include "XGXunFeiRealTimeSTTSubsystem.generated.h"



class FXGXunFeiConsumeVoiceRunnable;
class IWebSocket;
/**
 * UXGXunFeiRealTimeSTTSubsystem
 * Connect iFlyTek RealTime SST Web Api through WebSocket.
 */
UCLASS()
class  XGXUNFEISTT_API UXGXunFeiRealTimeSTTSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class FXGXunFeiConsumeVoiceRunnable;
	friend class UXGXunFeiLinkBPLibrary;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;
	virtual void Deinitialize()override;


protected:

	void XGBeginRealTimeSpeechToText(
		FXGXunFeiRealTimeSTTReqInfo& InRealTimeSTTReqInfo,
		FXGXunFeiInitRealTimeSTTDelegate InInitRealTimeSTTDelegate,
		FXGXunFeiRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate,
		FXGXunFeiRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate);
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

	EXGXunFeiRealTimeSTTStatus ReakTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;

	FXGXunFeiInitRealTimeSTTDelegate InitRealTimeSTTDelegate;

	FXGXunFeiRealTimeSTTTranslateDelegate RealTimeSTTTranslateDelegate;

	FXGXunFeiRealTimeSTTNoTranslateDelegate RealTimeSTTNoTranslateDelegate;

	TSharedPtr<IWebSocket> Socket;
	
	FCriticalSection SocketCriticalSection;

	TSharedPtr<FXGXunFeiConsumeVoiceRunnable> ConsumeVoiceRunnable;
};