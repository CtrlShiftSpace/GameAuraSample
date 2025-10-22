// Copyright Luke


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	// 避免自動啟動特效
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	// 確認此元件附屬的擁有者有 AbilitySystemComponent
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		// 如果有 ASC 就綁定事件
		AuraASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
		ActivateIfEquipped(AuraASC);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		// 檢查擁有者是否實作了 ICombatInterface
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda(
			[this](UAbilitySystemComponent* ASC)
			{
				if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC))
				{
					// 如果有 ASC 就綁定事件
					AuraASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
					ActivateIfEquipped(AuraASC);
				}
			}
		);
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	// 傳入的 AbilityTag 與此元件設定的 PassiveSpellTag 是否相同，相同時才做處理
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			// 如果傳入為 True 且特效尚未啟動，則啟動特效
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveNiagaraComponent::ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC)
{
	// 檢查是否在啟動時就已經給予被動能力
	const bool bStartupAbilitiesActivated = AuraASC->bStartupAbilitiesGiven;
	if (bStartupAbilitiesActivated)
	{
		// 如果已經給予被動能力，則檢查是否裝備，裝備時啟動特效
		if (AuraASC->GetStatusFromAbilityTag(PassiveSpellTag) == FAuraGameplayTags::Get().Abilities_Status_Equipped)
		{
			Activate();
		}
	}
}
