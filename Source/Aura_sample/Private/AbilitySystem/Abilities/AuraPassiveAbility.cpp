// Copyright Luke


#include "AbilitySystem/Abilities/AuraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		// 綁定被動技能停用的事件
		AuraASC->DeactivatePassiveAbility.AddUObject(this, &UAuraPassiveAbility::ReceiveDeactive);
	}
}

void UAuraPassiveAbility::ReceiveDeactive(const FGameplayTag& AbilityTag)
{
	// 檢查停用的被動技能 Tag 是否具備，如果有就結束此技能
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
