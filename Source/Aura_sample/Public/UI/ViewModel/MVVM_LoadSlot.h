// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

// 用於切換WidgetSwitcher的索引
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

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

	// 初始化讀取槽
	void InitializeSlot();

	// 設置玩家名稱
	UPROPERTY()
	FString PlayerName;

	// 設置讀取槽名稱
	UPROPERTY()
	FString LoadSlotName;

	// 設置讀取槽索引
	UPROPERTY()
	FString SlotIndex;
	
};
