// Copyright 2023 Xiao Gang. All Rights Reserved.
#include "XGiFlyTekAudioCaptureSubsystem.h"
#include "Generators/AudioGenerator.h"

UXGiFlyTekAudioCaptureSubsystem* UXGiFlyTekAudioCaptureSubsystem::iFlyTekAudioCaptureSubsystemPtr = nullptr;

bool UXGiFlyTekAudioCaptureSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGiFlyTekAudioCaptureSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	iFlyTekAudioCaptureSubsystemPtr = this;



}

void UXGiFlyTekAudioCaptureSubsystem::Deinitialize()
{
	StopCapturingAudio();
	iFlyTekAudioCaptureSubsystemPtr = nullptr;
	Super::Deinitialize();
}

UXGiFlyTekAudioCaptureSubsystem* UXGiFlyTekAudioCaptureSubsystem::Get()
{
	return iFlyTekAudioCaptureSubsystemPtr;
}

void UXGiFlyTekAudioCaptureSubsystem::StartCapturingAudio()
{
	if (!XGAudioCapture)
	{
		StopCapturingAudio();
	}

	XGAudioCapture = UAudioCaptureFunctionLibrary::CreateAudioCapture();
	AudioGeneratorHandle = XGAudioCapture->AddGeneratorDelegate(&UXGiFlyTekAudioCaptureSubsystem::OnAudioGenerate);

	if (XGAudioCapture)
	{
		ClearVoiceData();

		XGAudioCapture->StartCapturingAudio();
	}


}

void UXGiFlyTekAudioCaptureSubsystem::StopCapturingAudio()
{
	if (XGAudioCapture)
	{
		XGAudioCapture->StopCapturingAudio();
		XGAudioCapture->RemoveGeneratorDelegate(AudioGeneratorHandle);
		ClearVoiceData();
		XGAudioCapture = nullptr;
	}

}

bool UXGiFlyTekAudioCaptureSubsystem::IsCapturingAudio()
{
	if (XGAudioCapture)
	{
		return XGAudioCapture->IsCapturingAudio();
	}
	return false;


}

bool UXGiFlyTekAudioCaptureSubsystem::GetAudioCaptureDeviceInfo(FAudioCaptureDeviceInfo& OutInfo)
{
	if (XGAudioCapture)
	{
		return XGAudioCapture->GetAudioCaptureDeviceInfo(OutInfo);
	}

	return false;


}

void UXGiFlyTekAudioCaptureSubsystem::AppendVoiceData(const TArray<float>& InVoiceData)
{
	FScopeLock Lock(&VoiceDataSection);

	VoiceData.Append(InVoiceData);

}

bool UXGiFlyTekAudioCaptureSubsystem::GetVoiceData(TArray<float>& OutVoiceData)
{
	FScopeLock Lock(&VoiceDataSection);
	OutVoiceData.Empty();
	if (VoiceData.Num() > 1024)
	{
		OutVoiceData.Append(VoiceData.GetData(), 1024);
		VoiceData.RemoveAt(0, 1024);
		return true;
	}


	return false;
}

void UXGiFlyTekAudioCaptureSubsystem::ClearVoiceData()
{
	FScopeLock Lock(&VoiceDataSection);

	VoiceData.Empty();
}


void UXGiFlyTekAudioCaptureSubsystem::OnAudioGenerate(const float* InAudio, int32 NumSamples)
{
	UXGiFlyTekAudioCaptureSubsystem* AudioCaptureSubsystem = UXGiFlyTekAudioCaptureSubsystem::Get();

	static int32 IndexSend = 0;
	TArray<float> VoiceDataToApeend;
	int32 VoiceIndex = 0;

	if (NumSamples == 2048)
	{
		if (AudioCaptureSubsystem)
		{
			if (IndexSend == 0)
			{
				IndexSend++;
				for (int32 VoiceNum = 0; VoiceNum < 341; VoiceNum++)
				{
					VoiceDataToApeend.Add(InAudio[VoiceIndex]);
					VoiceIndex += 6;
				}

			}
			else if (IndexSend == 1)
			{
				IndexSend++;

				for (int32 VoiceNum = 341; VoiceNum < 682; VoiceNum++)
				{
					VoiceDataToApeend.Add(InAudio[VoiceIndex]);

					VoiceIndex += 6;
				}
			}
			else if (IndexSend == 2)
			{

				IndexSend = 0;

				for (int32 VoiceNum = 682; VoiceNum < 1024; VoiceNum++)
				{
					VoiceDataToApeend.Add(InAudio[VoiceIndex]);
					VoiceIndex += 6;
				}
			}
		}

	}
	else if (NumSamples == 960)
	{
		for (int32 VoiceNum = 0; VoiceNum < 160; VoiceNum++)
		{
			VoiceDataToApeend.Add(InAudio[VoiceIndex]);
			VoiceIndex += 6;
		}
	}
	else if (NumSamples == 480)
	{
		for (int32 VoiceNum = 0; VoiceNum < 160; VoiceNum++)
		{
			VoiceDataToApeend.Add(InAudio[VoiceIndex]);
			VoiceIndex += 3;
		}
	}
	else if (NumSamples == 320)
	{
		for (int32 VoiceNum = 0; VoiceNum < 160; VoiceNum++)
		{
			VoiceDataToApeend.Add(InAudio[VoiceIndex]);
			VoiceIndex += 2;
		}

	}
	else if (NumSamples == 160)
	{
		for (int32 VoiceNum = 0; VoiceNum < 160; VoiceNum++)
		{
			VoiceDataToApeend.Add(InAudio[VoiceIndex]);
			VoiceIndex += 1;
		}

	}



	AudioCaptureSubsystem->AppendVoiceData(VoiceDataToApeend);

}
