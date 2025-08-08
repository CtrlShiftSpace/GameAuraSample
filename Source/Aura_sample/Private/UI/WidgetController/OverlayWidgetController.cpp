// Copyright Luke


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// 設定Health值改變時的處理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()
	).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	
	// 設定Max Health值改變時的處理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// 設定Mana值改變時的處理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	// 設定MaxMana值改變時的處理
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilitiesGiven)
		{
			// 如果已經給予了Abilities，則直接呼叫函式
			OnInitializeStartupAbilities(AuraASC);
		}
		else
		{
			// 增加當AbilitiesGivenDelegate呼叫Broadcast時執行的程式
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		
		// 增加當EffectAssetTags呼叫Broadcast時執行的程式
		// 之所以使用Lambda是能簡化還要定義funciton名稱並指定的情況，相當於匿名function
		AuraASC->EffectAssetTags.AddLambda
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven)
	{
		return;
	}
	// 取得所有被賦予的能力，將其 Ability Info 再透過廣播方式傳遞給 Widget

	FForeachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)->
		void
		{
			// 從 AbilitySpec 取得 Ability Tag，再利用此 Tag 從 AbilityInfo 資料表中取得對應的 Ability Info
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			OnAbilityInfo.Broadcast(Info);
		});
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}
