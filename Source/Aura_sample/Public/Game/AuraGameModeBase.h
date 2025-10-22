// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// 儲存槽資料
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	// 取得SaveGame資料
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex);
	// 刪除指定存檔槽位
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	// 取得目前SaveGame資料
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	// 儲存遊戲進度資料
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);
	// 儲存世界狀態
	void SaveWorldState(UWorld* World);
	
	// 前往指定地圖關卡
	void TravelToMap(UMVVM_LoadSlot* Slot);
	
	// 指定 SaveGame 類別
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;
	
	// 遊戲開始時載入的地圖名稱
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	// 預設玩家生成點標籤
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	
	// 關於 TSoftObjectPtr 不會主動載入資源，只有在使用時才載入
	// 主要用於動態載入資源場景，避免一次載入全部地圖等
	// 遊戲關卡地圖名稱與對應地圖資源
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	// 覆寫玩家生成位置
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
protected:
	void BeginPlay() override;
};
