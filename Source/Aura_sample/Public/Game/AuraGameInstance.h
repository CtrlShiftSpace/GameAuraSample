// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// PlayerStartTag 屬性的名稱
	UPROPERTY()
	FName PlayerStartTag = FName();

	// 載入存檔槽位名稱
	UPROPERTY()
	FString LoadSlotName = FString();

	// 載入存檔槽位索引
	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
