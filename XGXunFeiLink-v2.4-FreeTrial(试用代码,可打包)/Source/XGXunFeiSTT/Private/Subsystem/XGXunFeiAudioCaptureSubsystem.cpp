// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGXunFeiAudioCaptureSubsystem.h"

#include "Generators/AudioGenerator.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/GameInstance.h"
#include "LogXGXunFeiSTT.h"

#include "XGXunFeiRealTimeSTTSubsystem.h"


UXGXunFeiAudioCaptureSubsystem* UXGXunFeiAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr = nullptr;

int32 UXGXunFeiAudioCaptureSubsystem::AudioCaptureSampleRate = 16000;
int32 UXGXunFeiAudioCaptureSubsystem::OnceSendFloatDataNum = 640;

TArray<float>  UXGXunFeiAudioCaptureSubsystem::NewAllVoiceData = {};



FAudioCaptureDeviceInfo  UXGXunFeiAudioCaptureSubsystem::AudioCaptureDeviceInfo;

FCriticalSection UXGXunFeiAudioCaptureSubsystem::VoiceDataSection;

bool UXGXunFeiAudioCaptureSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGXunFeiAudioCaptureSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	XunFeiAudioCaptureSubsystemPtr = this;


}

void UXGXunFeiAudioCaptureSubsystem::Deinitialize()
{

	StopCapturingAudio();
	XunFeiAudioCaptureSubsystemPtr = nullptr;
	Super::Deinitialize();


}

UXGXunFeiAudioCaptureSubsystem* UXGXunFeiAudioCaptureSubsystem::Get()
{
	return XunFeiAudioCaptureSubsystemPtr;
}

void UXGXunFeiAudioCaptureSubsystem::StartCapturingAudio(EXGXunFeiAudioCaptureType InAuidoCaptureType)
{
	if (!XGAudioCapture)
	{
		StopCapturingAudio();
	}

	XGAudioCapture = UAudioCaptureFunctionLibrary::CreateAudioCapture();

	if (XGAudioCapture)
	{
		ClearVoiceData();

		GetAudioCaptureDeviceInfo(AudioCaptureDeviceInfo);

		AudioGeneratorHandle = XGAudioCapture->AddGeneratorDelegate(&UXGXunFeiAudioCaptureSubsystem::OnAudioGenerate);

		switch (InAuidoCaptureType)
		{
		case EXGXunFeiAudioCaptureType::None:

			AudioCaptureSampleRate = 16000;

			break;
		case EXGXunFeiAudioCaptureType::RealtimeSTT:

			AudioCaptureSampleRate = 16000;

			break;

		case EXGXunFeiAudioCaptureType::VoiceDictation_8k:

			AudioCaptureSampleRate = 8000;

			OnceSendFloatDataNum = 320;

			break;

		case EXGXunFeiAudioCaptureType::VoiceDictation_16k:

			AudioCaptureSampleRate = 16000;


			break;
		default:
			break;
		}


		XGAudioCapture->StartCapturingAudio();
	}
	else
	{

		//UKismetSystemLibrary::PrintString(this, TEXT("Failed AudioCapture,please check it"));

		UE_LOG(LogXGXunFeiSTT, Warning, TEXT("[%s]:No Audio Device Captured ,Please Check it!"), *FString(__FUNCTION__));

	}
}

void UXGXunFeiAudioCaptureSubsystem::StopCapturingAudio()
{
	if (XGAudioCapture)
	{
		XGAudioCapture->StopCapturingAudio();
		XGAudioCapture->RemoveGeneratorDelegate(AudioGeneratorHandle);
		XGAudioCapture = nullptr;

	}

	ClearVoiceData();

}

bool UXGXunFeiAudioCaptureSubsystem::IsCapturingAudio()
{
	if (XGAudioCapture)
	{
		return XGAudioCapture->IsCapturingAudio();
	}

	return false;


}

bool UXGXunFeiAudioCaptureSubsystem::GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo)
{
	if (XGAudioCapture)
	{
		return XGAudioCapture->GetAudioCaptureDeviceInfo(OutInfo);
	}

	return false;
}

float UXGXunFeiAudioCaptureSubsystem::GetVolumeSize()
{
	return VolumeSize;
}

void UXGXunFeiAudioCaptureSubsystem::SetVolumeSize(float InVolumeSize)
{
	VolumeSize = InVolumeSize;
}

void UXGXunFeiAudioCaptureSubsystem::AppendVoiceData(const float* InAudio, int32 NumSamples)
{
	FScopeLock Lock(&VoiceDataSection);

	NewAllVoiceData.Append(InAudio, NumSamples);

}

bool UXGXunFeiAudioCaptureSubsystem::GetVoiceData(TArray<float>& OutVoiceData)
{
	FScopeLock Lock(&VoiceDataSection);

	OutVoiceData.Empty();

	if (AudioCaptureDeviceInfo.SampleRate == 48000 && (AudioCaptureDeviceInfo.NumInputChannels == 2 || AudioCaptureDeviceInfo.NumInputChannels == 1)
		|| AudioCaptureDeviceInfo.SampleRate == 16000 && (AudioCaptureDeviceInfo.NumInputChannels == 2 || AudioCaptureDeviceInfo.NumInputChannels == 1))
	{



		int32 AudioFix = AudioCaptureDeviceInfo.NumInputChannels * AudioCaptureDeviceInfo.SampleRate / AudioCaptureSampleRate;

		int32 HanldeAudioFloatNum = OnceSendFloatDataNum * AudioFix;


		if (NewAllVoiceData.Num() > HanldeAudioFloatNum)
		{

			for (int32 Index = 0; Index < OnceSendFloatDataNum; Index++)
			{
				OutVoiceData.Add(NewAllVoiceData[Index * AudioFix]);
			}

			NewAllVoiceData.RemoveAt(0, HanldeAudioFloatNum);

			//int32 VoiceTotalNum = NewAllVoiceData.Num();

			//AsyncTask(ENamedThreads::GameThread, [VoiceTotalNum]() {

			//	UE_LOG(LogXGXunFeiVoiceDictation, Warning, TEXT("--Num:[%d]-]"), VoiceTotalNum);

			//	});



			return true;
		}

	}
	else if (AudioCaptureDeviceInfo.SampleRate == 44100 && AudioCaptureDeviceInfo.NumInputChannels == 2
		|| AudioCaptureDeviceInfo.SampleRate == 44100 && AudioCaptureDeviceInfo.NumInputChannels == 1)
	{
		int32 HanldeAudioFloatNum = AudioCaptureDeviceInfo.SampleRate / 25 * AudioCaptureDeviceInfo.NumInputChannels;

		if (NewAllVoiceData.Num() > HanldeAudioFloatNum)
		{
			TArray<float> OriginData;

			int32 StepSize = AudioCaptureDeviceInfo.NumInputChannels;

			for (int32 Index = 0; Index < HanldeAudioFloatNum;)
			{
				Index += StepSize;
				OriginData.Add(NewAllVoiceData[Index]);

			}

			OutVoiceData = LinearResample(OriginData, OnceSendFloatDataNum);

			NewAllVoiceData.RemoveAt(0, HanldeAudioFloatNum);

			return true;
		}
	}
	else
	{
		check(false);
	}

	return false;
}

void UXGXunFeiAudioCaptureSubsystem::ClearVoiceData()
{

	FScopeLock Lock(&VoiceDataSection);

	NewAllVoiceData.Empty();

	if (UXGXunFeiAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr)
	{
		UXGXunFeiAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr->SetVolumeSize(0.f);
	}

}

void UXGXunFeiAudioCaptureSubsystem::OnAudioGenerate(const float* InAudio, int32 NumSamples)
{

	//FAudioCaptureDeviceInfo OutAudioCaptureDeviceInfo = AudioCaptureDeviceInfo;

	//AsyncTask(ENamedThreads::GameThread, [OutAudioCaptureDeviceInfo, NumSamples]() {

	//	UE_LOG(LogXGXunFeiVoiceDictation, Warning, TEXT("--NumCaputure:%d--AudioName:[%s]--AudioChannel:[%d]--AudioRate:[%d]"), NumSamples
	//		, *OutAudioCaptureDeviceInfo.DeviceName.ToString(), OutAudioCaptureDeviceInfo.NumInputChannels, OutAudioCaptureDeviceInfo.SampleRate);
	//	});

	if (NumSamples <= 0)
	{
		return;
	}

	float Volume = 0.0;

	//Volume calculation
	for (int32 VolumeCalculationIndex = 0; VolumeCalculationIndex < NumSamples; VolumeCalculationIndex++)
	{

		Volume += InAudio[VolumeCalculationIndex] * InAudio[VolumeCalculationIndex];
	}

	Volume = FMath::Sqrt(Volume / NumSamples);

	AsyncTask(ENamedThreads::GameThread, [Volume]() {

		if (UXGXunFeiAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr)
		{
			UXGXunFeiAudioCaptureSubsystem::XunFeiAudioCaptureSubsystemPtr->SetVolumeSize(Volume);
		}

		});

	UXGXunFeiAudioCaptureSubsystem::AppendVoiceData(InAudio, NumSamples);

}

TArray<float> UXGXunFeiAudioCaptureSubsystem::LinearResample(TArray<float>& InData, int32 OutDataSize)
{
	TArray<float> OutData;

	double ResampleRatio = (float)(InData.Num() - 1) / (float)(OutDataSize - 1);

	for (int32 i = 0; i < OutDataSize; ++i) {
		float Index = i * ResampleRatio;

		int32 LowerIndex = (int32)Index;
		int32 UpperIndex = FMath::Min((int32)Index + 1, InData.Num() - 1);
		float alpha = Index - LowerIndex;

		float interpolatedValue = (1.0 - alpha) * InData[LowerIndex] + alpha * InData[UpperIndex];
		OutData.Add(interpolatedValue);
	}

	return OutData;
}
