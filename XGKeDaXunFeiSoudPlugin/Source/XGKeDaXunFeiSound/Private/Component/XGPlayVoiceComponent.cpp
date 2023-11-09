// Copyright 2023 Xiao Gang. All Rights Reserved.
#include "XGPlayVoiceComponent.h"
#include "XGAudioCaptureSubsystem.h"

int32 UXGPlayVoiceComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	int i = 0;
	int j = 0;
	for (; i < 1024;)
	{	//1024 <= 2048
		//LLLLLLLL <=LRLRLRLRLRLR
		OutAudio[i] = UXGAudioCaptureSubsystem::InAudios[SoundIndex][j];

		

		i++;
		j = j + 2;
	}

	SoundIndex++;

	return 1024;


}

