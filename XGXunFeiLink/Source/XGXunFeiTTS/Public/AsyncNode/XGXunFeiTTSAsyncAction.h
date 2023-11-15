// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "XGXunFeiTTSReqType.h"

#include "XGXunFeiTTSAsyncAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FXGXunFeiTTSDelegate, bool, bResult, FString, Message, USoundWave*, SoundWavePtr);


class IWebSocket;
/**
 *	UXGXunFeiTTSAsyncAction
 *	Connect to iFlyTek Text To Speech Stream Webp Api
 *	
 */
UCLASS()
class XGXUNFEITTS_API UXGXunFeiTTSAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UXGXunFeiLinkBPLibrary;

public:

	UXGXunFeiTTSAsyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	/**
	 * XGXunFeiTextToSpeech BlueprintNode
	 * If you want to use in C++,please see XGXunFeiLinkBPLibrary.h .
	 * 
	 * @param WorldContextObject		WorldContext
	 * @param InText					The Str you want to Convert to Audio
	 * @param bInSaveToLocal			Whether to Save to local disk
	 * @param InSaveFileFullPath		The FilePath which the wmv file will save to .
	 * This directory must exist,and the file name must end with ".wav"  
	 * This path is absoult path!
	 * @param InXunFeiTTSReqInfo		About TTS Settins,more to look iflytesk document.you can choos which voice to say,and so on.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "XunFeiTextToSpeech",
		Keywords = "XG XunFei TTS Text To Speech "),
		Category = "XGXunFeiLink|TTS")
	static UXGXunFeiTTSAsyncAction* XGXunFeiTextToSpeech(UObject* WorldContextObject,
		const FString& InText,
		bool bInSaveToLocal,
		const FString& InSaveFileFullPath,
		FXGXunFeiTTSReqInfo InXunFeiTTSReqInfo);



public:

	virtual void Activate() override;


protected:

	void RealeaseResources();

	void CreateXunFeiTTSSocket();

	void CloseXunFeiTTsSocket();

	FORCEINLINE void SetXunFeiText(const FString& InXunFeiText) { XunFeiText = InXunFeiText; }
	FORCEINLINE void SetIsSaveToLocal(bool bInSaveToLocal) { bSaveToLocal = bInSaveToLocal; }
	FORCEINLINE void SetSaveFileFullPath(const FString& InSaveFilePath) { SaveFileFullPath = InSaveFilePath; }
	FORCEINLINE	void SetXunFeiTTSReqInfo(const FXGXunFeiTTSReqInfo& InXunFeiTTSReqInfo) { XunFeiTTSReqInfo = InXunFeiTTSReqInfo; }
protected:

	void OnConnected();
	void OnConnectionError(const FString& ErrorMessage);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);


public:

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiTTSDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiTTSDelegate OnFail;

private:

	TSharedPtr<IWebSocket> Socket;

	TArray<uint8> XunFeiAudioData;

	FString XunFeiText;

	bool bSaveToLocal = false;

	FString SaveFileFullPath = TEXT("");

	FXGXunFeiTTSReqInfo XunFeiTTSReqInfo;





};
