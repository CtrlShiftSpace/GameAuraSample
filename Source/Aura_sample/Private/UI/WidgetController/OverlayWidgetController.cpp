// Copyright Luke


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewValue, bool bLevelUp)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewValue, bLevelUp);
		}
	);
	
	// 設定Health值改變時的處理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetHealthAttribute()
	).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	
	// 設定Max Health值改變時的處理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// 設定Mana值改變時的處理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	// 設定MaxMana值改變時的處理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	if (GetAuraASC())
	{
		GetAuraASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			// 增加當AbilitiesGivenDelegate呼叫Broadcast時執行的程式
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		// 增加當EffectAssetTags呼叫Broadcast時執行的程式
		// 之所以使用Lambda是能簡化還要定義funciton名稱並指定的情況，相當於匿名function
		GetAuraASC()->EffectAssetTags.AddLambda
		(
			[this](const FGameplayTagContainer& AssetTags)->
			void
			{
				for (const FGameplayTag Tag : AssetTags)
				{
					// 假設Tag = Message.HealthPotion
					// 則 "Message.HealthPotion".MatchesTag("Message") 結果為 true
					// "Message".MatchesTag("Message.HealthPotion") 結果為 false
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					// 檢查是否符合Tag判斷，也就是當Tag只有在有Message才會執行以下內容
					if (Tag.MatchesTag(MessageTag))
					{
						// 使用Lambda 會無法直接呼叫該成員的函式，需在上方使用this才能知道傳入的類別
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						OnMessageWidgetRow.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	// 透過 AuraPlayerState 取得 LevelUpInfo
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	// 檢查 LevelUpInfo 是否有效，並且出錯時會顯示相關訊息
	checkf(LevelUpInfo, TEXT("無法找到 LevelUpInfo，請在 BP_AuraPlayerState 中設定 LevelUpInfo。"));
	// 透過目前經驗值取得當前等級
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	// 取得遊戲中最大等級
	int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		// 僅計算該等級所需的經驗值量
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		
		// 計算目前經驗值的百分比
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
		// 使用 static_cast 將 in32 轉換為 float，以便進行百分比計算
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	// 更新上次裝備的技能資訊
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	OnAbilityInfo.Broadcast(LastSlotInfo);

	// 更新目前裝備的技能資訊
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	OnAbilityInfo.Broadcast(Info);
	
}
