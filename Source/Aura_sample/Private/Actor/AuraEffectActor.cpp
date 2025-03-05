// Copyright Luke


#include "Actor/AuraEffectActor.h"
#include "Delegates/Delegate.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 如果對象是在Enemy且此效果不適用於Enemy上，就直接return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	// FGameplayEffectContextHandle 管理遊戲中的上下文資訊的結構，通常與行為、動作或事件相關聯
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	// Handle在變數命名可省略，這邊是為了避免混淆
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// 是否設置為Infinite模式
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	// 只有在設置為Infinite模式並且是需要在EndOverlap後移除該效果的，才會將其儲存起來
	if (bIsInfinite && InfiniteEffectAppilcationPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// 使用FActiveGameplayEffectHandle 與 TargetActor 來識別
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	if (!bIsInfinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// 如果對象是在Enemy且此效果不適用於Enemy上，就直接return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies)
	{
		return;
	}
	
	if (InstantEffectAppilcationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectAppilcationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectAppilcationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// 如果對象是在Enemy且此效果不適用於Enemy上，就直接return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies)
	{
		return;
	}
	
	if (InstantEffectAppilcationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectAppilcationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectAppilcationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// 取得目標的 UAbilitySystemComponent
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		// 驗證是否為UAbilitySystemComponent
		if (!IsValid(TargetASC)) return;

		// 建立一個空的陣列 HandlesToRemove 來存放要移除的效果
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			// 檢查效果是否屬於此角色
			if (TargetASC == HandlePair.Value)
			{
				// 將此效果從角色身上移除
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				// 先加入到待移除陣列中
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		// 從清單中刪除已移除的效果
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

