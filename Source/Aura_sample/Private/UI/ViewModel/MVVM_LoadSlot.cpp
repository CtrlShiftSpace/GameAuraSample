// Copyright Luke


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetPlayerName(const FText& InPlayerName)
{
	// 使用宏(macro)來設置 PlayerName 並觸發 FieldNotify
	// 用途是將變更通知給綁定的 Widget
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

FText UMVVM_LoadSlot::GetPlayerName() const
{
	return PlayerName;
}

FString UMVVM_LoadSlot::GetLoadSlotName() const
{
	return LoadSlotName;
}

FString UMVVM_LoadSlot::GetMapName() const
{
	return MapName;
}
