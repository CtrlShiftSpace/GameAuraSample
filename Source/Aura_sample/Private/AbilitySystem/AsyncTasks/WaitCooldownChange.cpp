// Copyright Luke


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	// 檢查 AbilitySystemComponent 與 InCooldownTag 是否有效，如果無效的情況下就結束 Task
	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// 註冊冷卻時間變化事件，用來知道 Cooldown Tag 是否有變化
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag, 
		EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCooldownChange, 
			&UWaitCooldownChange::CooldownTagChanged);

	// 用來了解是否有新的冷卻效果被添加到 AbilitySystemComponent 上
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC))
	{
		return;
	}
	// 刪除所有GameplayTag註冊綁定事件
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	// 設定物件狀態為 準備好被銷毀
	SetReadyToDestroy();
	// 標記此物件為垃圾物件，等待垃圾回收機制處理
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// 當技能冷卻效果被添加時，所要處理的項目

	// 取得 Spec 中所有的 Asset Tag
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	// 取得 Spec 中所有的 Granted Tag
	FGameplayTagContainer GrantedTag;
	SpecApplied.GetAllGrantedTags(GrantedTag);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTag.HasTagExact(CooldownTag))
	{
		// 建立一個 FGameplayEffectQuery 物件，由於需要傳入 FGameplayTagContainer，透過 GetSingleTagContainer 來將單一 Gameplay Tag 轉 Container
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		// 取得冷卻效果剩餘時間
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		// 取得所有剩餘時間的數量
		if (TimesRemaining.Num() > 0)
		{
			// 找出最大的剩餘時間
			float TimeRemaining = TimesRemaining[0];
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}

			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
