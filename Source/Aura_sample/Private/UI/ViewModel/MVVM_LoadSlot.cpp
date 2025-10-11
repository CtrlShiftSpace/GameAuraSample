// Copyright Luke


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	// TODO: 根據 Slot 資訊取得相關資料
	SetWidgetSwitcherIndex.Broadcast(2);
}
