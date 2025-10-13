// Copyright Luke


#include "UI/HUD/LoadScreenHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	// 創建讀取畫面ViewModel元件
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	// 初始化讀取槽位ViewModel元件
	LoadScreenViewModel->InitializeLoadSlots();
	
	// 創建並添加讀取畫面UI元件
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	// 在產生 LoadSlots 後呼叫
	LoadScreenWidget->BlueprintInitializeWidget();

	// 讀取存檔資料
	LoadScreenViewModel->LoadData();
}
