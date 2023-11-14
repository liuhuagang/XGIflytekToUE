// Copyright Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "XGiFlyTekTTSReqType.h"

#include "XGiFlyTekTTSAsyncAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FXGiFlyTekTTSDelegate, bool, bResult, FString, Message, USoundWave*, SoundWavePtr);


class IWebSocket;
/**
 *
 */
UCLASS()
class XGIFLYTEKTTS_API UXGiFlyTekTTSAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UXGiFlyTekToUEBPLibrary;

public:

	UXGiFlyTekTTSAsyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	/**
	 * XGiFlyTekTextToSpeech BlueprintNode
	 * If you want to use in C++,please see XGiFlyTekToUEBPLibrary.h .
	 * 
	 * @param WorldContextObject	WorldContext
	 * @param InText					The Str you want to Convert to Audio
	 * @param bInSaveToLocal			Whether to Save to local disk
	 * @param InSaveFileFullPath		The FilePath which the wmv file will save to .
	 * This directory must exist,and the file name must end with ".wav"  
	 * This path is absoult path!
	 * @param IniFlyTekTTSReqInfo		About TTS Settins,more to look iflytesk document.you can choos which voice to say,and so on.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "iFlyTekTextToSpeech",
		Keywords = "XG iFlyTek TTS Text To Speech "),
		Category = "XGiFlyTekToUE|TTS")
	static UXGiFlyTekTTSAsyncAction* XGiFlyTekTextToSpeech(UObject* WorldContextObject,
		const FString& InText,
		bool bInSaveToLocal,
		const FString& InSaveFileFullPath,
		FXGiFlyTekTTSReqInfo IniFlyTekTTSReqInfo);



public:

	virtual void Activate() override;


protected:

	void RealeaseResources();

	void CreateiFlyTekTTSSocket();

	void CloseiFlyTekTTsSocket();

	FORCEINLINE void SetiFlyTekText(const FString& IniFlyTekText) { iFlyTekText = IniFlyTekText; }
	FORCEINLINE void SetIsSaveToLocal(bool bInSaveToLocal) { bSaveToLocal = bInSaveToLocal; }
	FORCEINLINE void SetSaveFileFullPath(const FString& InSaveFilePath) { SaveFileFullPath = InSaveFilePath; }
	FORCEINLINE	void SetiFlyTekTTSReqInfo(const FXGiFlyTekTTSReqInfo& IniFlyTekTTSReqInfo) { iFlyTekTTSReqInfo = IniFlyTekTTSReqInfo; }
protected:

	void OnConnected();
	void OnConnectionError(const FString& ErrorMessage);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);


public:

	UPROPERTY(BlueprintAssignable)
	FXGiFlyTekTTSDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGiFlyTekTTSDelegate OnFail;

private:

	TSharedPtr<IWebSocket> Socket;

	TArray<uint8> iFlyTekAudioData;

	FString iFlyTekText;

	bool bSaveToLocal = false;

	FString SaveFileFullPath = TEXT("");

	FXGiFlyTekTTSReqInfo iFlyTekTTSReqInfo;





};
