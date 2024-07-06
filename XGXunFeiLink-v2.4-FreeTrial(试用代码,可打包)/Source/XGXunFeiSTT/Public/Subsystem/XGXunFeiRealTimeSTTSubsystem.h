// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"


#include "XGXunFeiRealTimeSTTType.h"
#include "XGXunFeiRealTimeSTTReqType.h"

#include "XGXunFeiRealTimeSTTSubsystem.generated.h"


class IWebSocket;
class FXGXunFeiConsumeVoiceRunnable;
class UXGXunFeiLinkBPLibrary;
class UXGXunFeiAudioCaptureSubsystem;
class UXGXunFeiVoiceDictationSubsystem;

/**
 * UXGXunFeiRealTimeSTTSubsystem
 * Connect iFlyTek RealTime SST WebAPI through WebSocket.
 */
UCLASS()
class  XGXUNFEISTT_API UXGXunFeiRealTimeSTTSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class FXGXunFeiConsumeVoiceRunnable;
	friend class UXGXunFeiLinkBPLibrary;
	friend class UXGXunFeiAudioCaptureSubsystem;
	friend class UXGXunFeiVoiceDictationSubsystem;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;
	virtual void Deinitialize()override;


protected:

	bool IsRealTimeSTTReady();

	FGuid XGBeginRealTimeSpeechToText(
		FString InSTTAppID,
		FString InSTTAPIKey,
		FXGXunFeiRealTimeSTTReqInfo& InRealTimeSTTReqInfo,
		FXGXunFeiInitRealTimeSTTDelegate InRealTimeSTTInitDelegate,
		FXGXunFeiRealTimeSTTRespDelegate InRealTimeSTTRespDelegate,
		FXGXunFeiRealTimeSTTCloseDelegate InRealTimeSTTCloseDelegate);

	void XGStopRealTimeSpeechToText();

	void XGForceToStopRealTimeSPeechToText(bool IsDeinitialize=false);

	void CallInitRealTimeSTTDelegate(FGuid InSTTAsyncID, bool bInInitResult, FString InInitRespMessage, FXGXunFeiRealTimeSTTInitInformation InRealTimeSTTInitInformation);

	void CallRealTimeSTTRespDelegate( FXGXunFeiRealTimeSTTInformation  InRealTimeSTTInformation);

	void CallRealTimeSTTCloseDelegate(FGuid InSTTAsyncID, int32 InStatusCode, FString InReason,bool bInWasClean);

	void SendVoiceData(const TArray<float>& InVoiceData);

	void RealseaseVoiceGenerateRunnale();

	void RealeaseVoiceConsumeRunnable();

	void RealeaseWebSocket();

	void RealeaseSTTInfo();

	void EndSendVoiceData();





protected:

	void OnConnected();

	void OnConnectionError(const FString& ErrorMessage);

	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	void OnMessage(const FString& Message);

	void OnMessageSent(const FString& MessageString);


protected:

	static UXGXunFeiRealTimeSTTSubsystem* RealTimeSTTSubsystemPtr;

	EXGXunFeiRealTimeSTTStatus RealTimeSTTStatus = EXGXunFeiRealTimeSTTStatus::Ready;

	FGuid STTAsyncID = FGuid();

	FString STTAppID=TEXT("");

	FString STTAPIKey=TEXT("");

	FXGXunFeiInitRealTimeSTTDelegate RealTimeSTTInitDelegate;

	FXGXunFeiRealTimeSTTRespDelegate RealTimeSTTRespDelegate;

	FXGXunFeiRealTimeSTTCloseDelegate RealTimeSTTCloseDelegate;

	TSharedPtr<IWebSocket> Socket;

	TSharedPtr<FXGXunFeiConsumeVoiceRunnable> ConsumeVoiceRunnable;

	FRunnableThread* RunnableThread;


};