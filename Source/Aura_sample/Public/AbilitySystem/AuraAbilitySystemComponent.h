// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
// 用來在當被賦予能力時，發出廣播用途
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
// 用來遍歷所有能力
DECLARE_DELEGATE_OneParam(FForeachAbility, const FGameplayAbilitySpec&);
// 用來處理技能改變時的資訊傳遞
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status Tag*/, int32 /*AbilityLevel*/);
// 進行裝備技能時的資訊傳遞
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);
// 用來處理被動技能停用時的資訊傳遞
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/);

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEquipped AbilityEquipped;
	FDeactivatePassiveAbility DeactivatePassiveAbility;

	void AddCharacterAbilites(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilites(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(FForeachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);
	// 檢查裝備技能欄位是否為空
	bool SlotIsEmpty(const FGameplayTag& Slot);
	// 檢查指定的 AbilitySpec 是否有綁定到指定的欄位
	static bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	// 檢查指定的 AbilitySpec 是否有綁定到任何欄位
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	// 由指定的 Slot 取得該欄位綁定的 AbilitySpec
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);
	// 檢查該能力是否為被動技能
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	// 將指定的 AbilitySpec 綁定到指定的 Slot
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	void UpdateAbilityStatus(int32 Level);

	// 由 client 呼叫，並在 server 端執行花費技能點數動作
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	// 由 client 呼叫，並在 server 端執行裝備技能動作
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);

	// 由 server 呼叫，並在 client 端執行裝備技能動作
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);
	
	// 由 AbilityTag 取得該能力的描述文字，包含下一等級的描述
	bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	// 移除指定的裝備欄位的技能
	static void ClearSlot(FGameplayAbilitySpec* Spec);

	// 指定 Slot 的技能並移除
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);

	// 檢查指定的能力是否有綁定到指定的 Slot 標籤
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);
protected:

	virtual void OnRep_ActivateAbilities() override;

	// 標記函數為RPC，這邊為 Client RPC，從伺服器呼叫，並在指定的客戶端上執行。
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
