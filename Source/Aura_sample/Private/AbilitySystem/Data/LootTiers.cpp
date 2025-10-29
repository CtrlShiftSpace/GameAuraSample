// Copyright Luke


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	// 要回傳的 LootItem 物件
	TArray<FLootItem> ReturnItems;

	// 根據每項戰利品，隨機產生
	for (FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.MaxNumberToSpawn; ++i)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				ReturnItems.Add(NewItem);
			}
		}
	}
	
	return ReturnItems;
}
