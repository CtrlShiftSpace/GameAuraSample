// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

// 存檔槽位狀態
UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

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

	// 關卡地圖名稱
	UPROPERTY()
	FString MapName = FString("Default Map Name");

	// 儲存玩家生成點標籤
	UPROPERTY()
	FName PlayerStartTag;

	// 存檔槽位狀態
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = ESaveSlotStatus::Vacant;

	/* 儲存 Player 資訊 */

	// 玩家等級
	UPROPERTY()
	int32 PlayerLevel = 0;

	// 玩家經驗值
	UPROPERTY()
	int32 XP = 0;

	// 技能點數
	UPROPERTY()
	int32 SpellPoints = 0;

	// 屬性點數
	UPROPERTY()
	int32 AttributePoints = 0;

	/* 儲存 Player Attribute Set 資訊 */
	
	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;

	UPROPERTY()
	float Resilience = 0.f;

	UPROPERTY()
	float Vigor = 0.f;

};
