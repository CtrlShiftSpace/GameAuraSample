// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;
// 存檔槽位狀態
UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	// Actor 名稱
	UPROPERTY()
	FName ActorName = FName();

	// 紀錄位置旋轉等資訊
	UPROPERTY()
	FTransform Transform = FTransform();

	// 紀錄 Actor 的 Serialize 資料
	UPROPERTY()
	TArray<uint8> Bytes;
};

// 比較兩個 FSavedActor 結構中的 ActorName 是否相等
inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	// 地圖資產名稱
	UPROPERTY()
	FString MapAssetName = FString();

	// 儲存地圖中的 Actor 資訊
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	// 技能 Tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	// 技能狀態 Tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityStatus = FGameplayTag();

	// 技能裝備欄位
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilitySlot = FGameplayTag();

	// 技能類型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	// 技能等級
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AbilityLevel;
};

inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	// 比較兩個 FSavedAbility 結構的 AbilityTag 是否相等
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

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

	// 是否為第一次載入遊戲
	UPROPERTY()
	bool bFirstTimeLoadIn = true;
	
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
	
	// 儲存擁有技能
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	// 儲存地圖關卡中的資訊
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	// 根據地圖名稱取得已儲存的地圖資訊
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);

	// 檢查是否有儲存指定名稱的地圖資訊
	bool HasMap(const FString& InMapName);
};
