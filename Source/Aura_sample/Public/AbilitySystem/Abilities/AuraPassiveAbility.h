// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	
	// 被動技能啟動時會呼叫
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 接收被動技能停用的事件
	void ReceiveDeactive(const FGameplayTag& AbilityTag);
};
