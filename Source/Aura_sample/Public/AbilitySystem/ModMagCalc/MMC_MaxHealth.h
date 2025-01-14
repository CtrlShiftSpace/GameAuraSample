// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	// 建構式，設定需要的Ability System屬性
	UMMC_MaxHealth();
	// 覆寫CalculateBaseMagnitude_Implementation
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:

	FGameplayEffectAttributeCaptureDefinition VigorDef;

};
