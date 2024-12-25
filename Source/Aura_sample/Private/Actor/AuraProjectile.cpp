// Copyright Luke


#include "Actor/AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAuraProjectile::AAuraProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	// 啟用碰撞，但僅用查詢，所以物體不會阻擋，可用於重疊檢測
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// 預設所有碰撞channel 為忽略
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 移動物體會觸發重疊事件
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	// 靜止物體會觸發重疊事件
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	// 角色會觸發重疊事件
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

