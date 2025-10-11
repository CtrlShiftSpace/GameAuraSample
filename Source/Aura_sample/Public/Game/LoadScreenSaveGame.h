// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	// 存檔槽位名稱
	UPROPERTY()
	FString SlotName = FString();

	// 存檔槽位索引
	UPROPERTY()
	int32 SlotIndex = 0;

	// 玩家名稱
	UPROPERTY()
	FString PlayerName = FString("Default Name");
};
