// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <IWebSocket.h>
#include "Thread/ConsumeSoundRunnable.h"
#include "XGKeDaXunFeiSoundBPLibrary.h"
#include "XGRealTimeSpeechToTextRequestType.h"
#include "XGKeDaXunFeiSocketSubsystem.generated.h"

class UXGAudioCaptureSubsystem;
/**
 *
 */
UCLASS()
class XGKEDAXUNFEISOUND_API UXGKeDaXunFeiSocketSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
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
	 void XGBeginSpeachToText(FXGBeginSpeachToTextDelegate InXGBeginSpeachToTextDelegate, FXGSpeachToTextDelegate InXGSpeachToTextDelegat);
	 void XGBeginSpeachToTextConfig(FXGRealTimeSpeechToTextRequestType InXGRealTimeSpeechToTextType, 
	 FXGBeginSpeachToTextDelegate InXGBeginSpeachToTextDelegate, 
	 FXGSpeachToTextDelegate InXGSpeachToTextDelegat,
	 FXGSpeachToTextWithoutTranslateDelegate InXGSpeachToTextWithoutTranslateDelegate = FXGSpeachToTextWithoutTranslateDelegate()
	 );

	 void XGStopSpeachToText();


public:
	UFUNCTION(BlueprintCallable)
		void CreateSocket();
	UFUNCTION(BlueprintCallable)
		void CloseSocket();


public:

	 static void SendVoiceData(const float* InAudio, int32 NumSamples);
	 static void EndSendVoiceData();

	UFUNCTION(BlueprintCallable)
	void StopSenVoiceData();

protected:

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);

	void OnMessageFullConfig(const FString& Message);


protected:
	static	TSharedPtr<IWebSocket> Socket ;
	FString ServerURL = TEXT("");
	FString ServerProtocol = TEXT("");

	FString Appid;
	FString APIKey;

	static	bool bSending ;


	bool bSpeechToText = false;
	FXGBeginSpeachToTextDelegate XGBeginSpeachToTextDelegate;
	FXGSpeachToTextDelegate XGSpeachToTextDelegate;
	FXGSpeachToTextWithoutTranslateDelegate XGSpeachToTextWithoutTranslateDelegate;

	TSharedPtr< FConsumeSoundRunnable> VoiceRunnable;
public:
	static UXGAudioCaptureSubsystem* XGAudioCaptureSubsystem;
};
