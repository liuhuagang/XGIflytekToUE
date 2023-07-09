// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <IWebSocket.h>
#include "Thread/ConsumeSoundRunnable.h"
#include "XGKeDaXunFeiSoundBPLibrary.h"
#include "XGTTSSubsystem.generated.h"

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
	static TArray<float> FinalUEData;


protected:

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);

protected:
	TSharedPtr<IWebSocket> Socket ;
	FString ServerURL = TEXT("");
	FString ServerProtocol = TEXT("");


protected:

	FString HMACSHA256(FString APPSecreet, FString Data);
};
