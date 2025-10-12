// Copyright Luke


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	// TODO: 根據 Slot 資訊取得相關資料
	SetWidgetSwitcherIndex.Broadcast(2);
}

void UMVVM_LoadSlot::SetPlayerName(const FText& InPlayerName)
{
	// 使用宏(macro)來設置 PlayerName 並觸發 FieldNotify
	// 用途是將變更通知給綁定的 Widget
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

FText UMVVM_LoadSlot::GetPlayerName() const
{
	return PlayerName;
}
