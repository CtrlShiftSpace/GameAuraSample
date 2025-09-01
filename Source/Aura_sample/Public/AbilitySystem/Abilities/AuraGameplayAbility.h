// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	// 取得技能在不同等級的描述說明
	virtual FString GetDescription(int32 Level); 
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:
	
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
};
