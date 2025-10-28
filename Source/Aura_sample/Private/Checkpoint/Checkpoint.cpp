// Copyright Luke


#include "Checkpoint/Checkpoint.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

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

	// 建立移動到的位置元件
	MoveToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(GetRootComponent());
}

bool ACheckpoint::ShouldLoadTransform_Implementation()
{
	return false;
}

void ACheckpoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 如果偵測到進入的是玩家
	if (OtherActor->Implements<UPlayerInterface>())
	{
		// 設定到達此存檔點位置
		bReached = true;
		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// 存檔所在關卡名稱
			const UWorld* World = GetWorld();
			FString MapName = World->GetMapName();
			// 移除關卡前綴名稱
			MapName.RemoveFromStart(World->StreamingLevelsPrefix);

			// 存檔遊戲世界狀態
			AuraGM->SaveWorldState(GetWorld(), MapName);
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		// 處理 Checkpoint 物件發光效果
		HandleGlowEffects();
	}
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	if (bBindOverlapCallback)
	{
		// 綁定重疊事件
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
	}
	// 設定 CustomDepthStencil 值
	CheckpointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
}

void ACheckpoint::HighlightActor_Implementation()
{
	if (!bReached)
	{
		CheckpointMesh->SetRenderCustomDepth(true);
	}
}

void ACheckpoint::UnHighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false);
}

void ACheckpoint::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = MoveToComponent->GetComponentLocation();
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
