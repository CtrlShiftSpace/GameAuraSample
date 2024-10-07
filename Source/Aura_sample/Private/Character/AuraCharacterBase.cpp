// Copyright Luke


#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// 如果開啟(true)的話，每一幀會調用Tick()，這邊關閉(false)可以節省效能
	PrimaryActorTick.bCanEverTick = false;

	// 建立 component 要使用 CreateDefaultSubobject
	// 帶入參數的SubobjectName的類型是FName，一般需要使用TEXT物件傳入，但仍可接受常規字串文字
	// 但使用TEXT()會因為使用寬字節會占用較多記憶體
	// 並對CPU有額外開銷且產生不必要的轉換，因此使用字串傳入
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// 將一個component附加到另一個component上
	// Inparent: 父component
	// InSocketName: 附加到父component 預設值為NAME_None
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	// 設定碰撞(無碰撞)
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
