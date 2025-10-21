// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

// 用於切換WidgetSwitcher的索引
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
// 用於啟用或禁用選擇槽位按鈕
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	// 切換 WidgetSwitcher 的 Delegate 變數
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	// 啟用或禁用選擇槽位按鈕的 Delegate 變數
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;

	// 初始化讀取槽
	void InitializeSlot();

	// 設置讀取槽索引
	UPROPERTY()
	int32 SlotIndex;

	// 讀取槽狀態
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	/* 由於 FieldNotify 需要 Setter */
	
	// 設定 PlayerName 
	void SetPlayerName(const FText& InPlayerName);
	// 設定 LoadSlotName
	void SetLoadSlotName(FString InLoadSlotName);
	// 設定 MapName
	void SetMapName(FString InMapName);
	// 設定 PlayerLevel
	void SetPlayerLevel(int32 InLevel);
	

	// 用來記錄 Player Start Tag 以利於得知要從哪個 Player Start 生成玩家
	UPROPERTY()
	FName PlayerStartTag;

	/* 由於 FieldNotify 需要 Getter */
	
	// 取得 PlayerName 
	FText GetPlayerName() const;
	// 取得 LoadSlotName
	FString GetLoadSlotName() const;
	// 取得 MapName
	FString GetMapName() const;
	// 取得 PlayerLevel
	int32 GetPlayerLevel() const;
	

private:

	/* Field Notifies */
	
	// 設置讀取槽名稱
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString LoadSlotName;

	// 設置玩家名稱
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FText PlayerName;

	// 角色等級
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString MapName;
	
	// 角色等級
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	int32 PlayerLevel;
};
