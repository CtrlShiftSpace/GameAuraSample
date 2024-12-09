// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * Singleton containing native Gameplay Tags
 */

struct FAuraGameplayTags
{

public:
	// 返回靜態實例
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

private:
	// 存儲唯一的實例
	static FAuraGameplayTags GameplayTags;

};
