// Copyright Luke


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Aura_sample/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
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
	// 儲存玩家生成點標籤
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->PlayerLevel = LoadSlot->GetPlayerLevel();
	LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;

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

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	// 更新玩家生成點標籤
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName)
{
	// 儲存當前世界的地圖名稱
	FString WorldName = World->GetMapName();
	// 移除地圖名稱的前綴字串
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	// 取得遊戲實例
	UAuraGameInstance* AuraGI = GetGameInstance<UAuraGameInstance>();
	check(AuraGI);

	// 取得存檔資料
	if (ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex)))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}

		// 檢查是否有儲存過此地圖關卡資訊
		if (!SaveGame->HasMap(WorldName))
		{
			// 尚未儲存過此地圖關卡，則新建 FSavedMap 加入到 SavedMaps 陣列中
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		// 取得對應地圖名稱的已儲存地圖資訊
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		// 清空該地圖名稱已儲存的 Actor 資訊
		SavedMap.SavedActors.Empty();

		// 遍歷世界中的所有 Actor
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			// 檢查 Actor 存在並實作了 USaveInterface 介面
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>())
			{
				continue;
			}

			// 設定要儲存的資訊
			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			// 創建一個記憶體寫入器 (FMemoryWriter) 負責將資料寫入到記憶體緩衝區
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			// 創建一個代理序列化器 (FObjectAndNameAsStringProxyArchive)
			// 第二個參數 'true' 表示這個 Archive 處於「儲存/寫入」模式。
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			// 用來標記正在做的事情是屬於遊戲存檔/讀檔系統的一部分
			// 只有當設定 ArIsSaveGame = true，才會對使用 UPROPERTY(SaveGame) 標籤的屬性資料被寫入到 Archive
			Archive.ArIsSaveGame = true;
			// 使用 Actor 的 Serialize 方法將 Actor 的狀態序列化到 Archive
			Actor->Serialize(Archive);
			// 將序列化後的 SavedActor 加入到 SavedMap 的 SavedActors 陣列中
			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				// 用更新後的 SavedMap 替換原本的地圖資訊
				MapToReplace = SavedMap;
				break;
			}
		}
		// 最後將更新後的 SaveGame 儲存回指定的槽位
		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	// 取得當前世界的地圖名稱
	FString WorldName = World->GetMapName();
	// 移除地圖名稱的前綴字串
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	// 取得遊戲實例
	UAuraGameInstance* AuraGI = GetGameInstance<UAuraGameInstance>();
	check(AuraGI);

	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		// 取得存檔資料
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load slot."));
			return;
		}
		
		// 遍歷世界中的所有 Actor
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			// 檢查 Actor 是否實作了 USaveInterface 介面
			if (!Actor->Implements<USaveInterface>())
			{
				continue;
			}

			// 取得所有 SavedActor 資訊
			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					// 創建一個記憶體讀取器 (FMemoryReader) 負責從記憶體緩衝區讀取資料
					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					// 使用 Actor 的 Serialize 方法將序列化的資料讀取回 Actor
					Actor->Serialize(Archive);

					// 呼叫 LoadActor 方法
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
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

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

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
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
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

void AAuraGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	// 讀取上次存檔資料
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame))
	{
		return;
	}

	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// 將預設地圖加入 Maps 中
	Maps.Add(DefaultMapName, DefaultMap);
}
