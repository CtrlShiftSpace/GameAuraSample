// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_SAMPLE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangeSignature CooldownEnd;

	// 定義內部使用函數
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);
	// 結束Task
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);

};
