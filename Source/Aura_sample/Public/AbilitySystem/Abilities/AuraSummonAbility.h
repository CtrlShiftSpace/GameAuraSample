// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:

	// 取得召喚所生成位置的陣列
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	// 取得隨機生成隨從類別
	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	// 隨從數量
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	// 隨從類別
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	// 生成位置最小距離
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	// 生成位置最大距離
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 250.f;

	// 生成位置所在角度範圍
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90;

};
