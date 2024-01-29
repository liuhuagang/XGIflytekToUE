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
	friend class UXGXunFeiAudioCaptureSubsystem;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;
	virtual void Deinitialize()override;


protected:

	void XGBeginRealTimeSpeechToText(
		FString InSTTAppID,
		FString InSTTAPIKey,
		FXGXunFeiRealTimeSTTReqInfo& InRealTimeSTTReqInfo,
		FXGXunFeiInitRealTimeSTTDelegate InInitRealTimeSTTDelegate,
		FXGXunFeiRealTimeSTTNoTranslateMiddleDelegate InRealTimeSTTNoTranslateMiddleDelegate,
		FXGXunFeiRealTimeSTTNoTranslateDelegate InRealTimeSTTNoTranslateDelegate,
		FXGXunFeiRealTimeSTTTranslateDelegate InRealTimeSTTTranslateDelegate);
	void XGStopRealTimeSpeechToText();


	void CallInitRealTimeSTTDelegate(bool bInitResult,FString InitMessage);

	void CallRealTimeSTTNoTranslateMiddleDelegate(FString InSrcText);

	void CallRealTimeSTTNoTranslateDelegate(FString InSrcText);

	void CallRealTimeSTTTranslateDelegate(FString InSrcText, FString InDstText);

	void SendVoiceData(const TArray<float>& InVoiceData);

	void RealeaseVoiceConsumeRunnable();

	void EndSendVoiceData();



protected:

	void OnConnected();
	void OnConnectionError(const FString& ErrorMessage);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);


protected:

	FString STTAppID=TEXT("");

	FString STTAPIKey=TEXT("");


	EXGXunFeiRealTimeSTTStatus ReakTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;

	FXGXunFeiInitRealTimeSTTDelegate InitRealTimeSTTDelegate;

	FXGXunFeiRealTimeSTTNoTranslateMiddleDelegate RealTimeSTTNoTranslateMiddleDelegate;

	FXGXunFeiRealTimeSTTNoTranslateDelegate RealTimeSTTNoTranslateDelegate;

	FXGXunFeiRealTimeSTTTranslateDelegate RealTimeSTTTranslateDelegate;





	TSharedPtr<IWebSocket> Socket;

	FCriticalSection SocketCriticalSection;

	TSharedPtr<FXGXunFeiConsumeVoiceRunnable> ConsumeVoiceRunnable;

	FRunnableThread* RunnableThread;
};