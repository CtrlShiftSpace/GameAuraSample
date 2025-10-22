// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPassiveNiagaraComponent();

	// 設定被動技能的 GameplayTag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	virtual void BeginPlay() override;
	// 被動技能啟動與停用的事件
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
	// 檢查是否裝備該被動技能並啟用特效
	void ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC);
};
