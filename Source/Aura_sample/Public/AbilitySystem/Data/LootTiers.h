// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	// 戰利品類別
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning")
	TSubclassOf<AActor> LootClass;

	// 掉落戰利品機率
	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	float ChanceToSpawn = 0.f;

	// 戰利品最大數量
	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	int32 MaxNumberToSpawn = 0.f;

	// 是否覆蓋戰利品等級
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning")
	bool bLootLevelOverride = true;
};

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:

	// 取得戰利品陣列
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();

	UPROPERTY(EditDefaultsOnly, Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItems;
};
