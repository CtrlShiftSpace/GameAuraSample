// Copyright Luke


#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	AAuraGameModeBase::DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	// 創建一個新的 SaveGame 物件
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	// 將 SaveGame 物件轉換為 ULoadScreenSaveGame 類型
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	// 儲存玩家名稱
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName().ToString();
	LoadScreenSaveGame->SaveSlotStatus = ESaveSlotStatus::Taken;
	// 儲存所在關卡地圖名稱
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();

	// 將 SaveGameObject 儲存
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex)
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	return Cast<ULoadScreenSaveGame>(SaveGameObject);
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	// 檢查存檔槽位是否存在，如果存在就刪除它
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	// 取得槽位的名稱和索引
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;
	// 使用 UGameplayStatics::OpenLevelBySoftObjectPtr 來載入指定的地圖關卡
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Actors;
	// 取得場景中所有 PlayerStart 類別的 Actor
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		// 預設取得第一個 Actor
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == FName("TheTag"))
				{
					// 如果找到帶有指定標籤的 PlayerStart，則選擇該 Actor
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// 將預設地圖加入 Maps 中
	Maps.Add(DefaultMapName, DefaultMap);
}
