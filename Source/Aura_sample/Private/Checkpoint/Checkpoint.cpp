// Copyright Luke


#include "Checkpoint/Checkpoint.h"
#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"

ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	// 建立 Mesh
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	// Sphere 放置在 CheckpointMesh 下
	Sphere->SetupAttachment(CheckpointMesh);
	// 啟用碰撞，但僅用查詢，所以物體不會阻擋，可用於重疊檢測
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// 預設所有碰撞channel 為忽略
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 角色會觸發重疊事件
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 如果偵測到進入的是玩家
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		// 處理 Checkpoint 物件發光效果
		HandleGlowEffects();
	}
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	// 綁定重疊事件
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
	
}

void ACheckpoint::HandleGlowEffects()
{
	// 設定取消碰撞
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 建立動態 MaterialInstance
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	// 設定動態建立的 MaterialInstance
	CheckpointMesh->SetMaterial(0, DynamicMaterialInstance);
	// 呼叫 CheckpointReached 事件進行存檔
	CheckpointReached(DynamicMaterialInstance);
}
