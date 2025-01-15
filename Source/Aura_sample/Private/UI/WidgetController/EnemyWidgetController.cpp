// Copyright Luke


#include "UI/WidgetController/EnemyWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UEnemyWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(EnemyAttributeInfo);

	for (auto& Pair : AS->TagToAttributes)
	{
		BroadcastEnemyAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UEnemyWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(EnemyAttributeInfo);

	for (auto& Pair : AS->TagToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastEnemyAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UEnemyWidgetController::BroadcastEnemyAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = EnemyAttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	EnemyAttributeInfoDelegate.Broadcast(Info);
}
