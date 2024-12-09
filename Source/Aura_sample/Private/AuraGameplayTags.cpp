// Copyright Luke


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block Chance."));
}

// 提供靜態變數初始化，並分配記憶體位置
FAuraGameplayTags FAuraGameplayTags::GameplayTags;
