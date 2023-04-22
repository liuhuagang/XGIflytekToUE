
#pragma once

#include "CoreMinimal.h"
#include "Components/SynthComponent.h"
#include "XGPlayVoiceComponent.generated.h"


UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnableComponent))
class XGKEDAXUNFEISOUND_API UXGPlayVoiceComponent : public USynthComponent
{
	GENERATED_BODY()



	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;

	int32 SoundIndex = 0;

};