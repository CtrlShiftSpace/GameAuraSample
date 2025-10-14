// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

// 用於得知當前選擇槽位的 Delegate 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	// 初始化讀取槽位ViewModel
	void InitializeLoadSlots();

	// 當前選擇槽位的 Delegate 變數
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	// 讀取槽位ViewModel類別
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	// 根據索引獲取讀取槽位ViewModel
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	// 新建存檔按鈕被按下的事件
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot);

	// 開始遊戲按鈕被按下的事件
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	// 選擇存檔按鈕被按下的事件
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	// 刪除存檔按鈕被按下的事件
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();
	
	// 載入存檔資料
	void LoadData();

	// 設置讀取槽位數量
	void SetNumLoadSlots(int32 InNumLoadSlots);

	// 獲取讀取槽位數量
	int32 GetNumLoadSlots() const;
private:

	// 用來保存讀取槽位ViewModel TMap
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	// 當前選擇的讀取槽位ViewModel
	UPROPERTY()
	UMVVM_LoadSlot* SelectedSlot;

	/* Field Notifies */
	
	// 讀取槽數量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	int32 NumLoadSlots;
};
