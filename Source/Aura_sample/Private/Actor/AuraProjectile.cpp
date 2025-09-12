// Copyright Luke


#include "Actor/AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Aura_sample/Aura_sample.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

AAuraProjectile::AAuraProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	// 設定碰撞物件類型
	Sphere->SetCollisionObjectType(ECC_Projectile);
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
	// 當超過此時間(秒)時，此物體會自動Destroy
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	
	// 因為這個物件不屬於Actor ，而是 world 環境中產生的，因此需要用指標記錄起來
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	SetReplicateMovement(true);
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	
	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}

	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor)
	{
		return;
	}

	if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor))
	{
		return;
	}

	if (!bHit)
	{
		OnHit();
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// 計算衝擊的方向與力道
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			// 擊退的機率
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				// 設定擊退的旋轉(做法1)
				// const FVector KnockbackDirection = GetActorForwardVector().RotateAngleAxis(45.f, GetActorRightVector());

				// 設定擊退的旋轉(做法2)
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const FVector KnockbackDirection = Rotation.Vector();
				
				// 設定擊退的向量
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			// 因為原來沒有設定TargetAbilitySystemComponent，所以需要在觸碰到角色時設定目標的 AbilitySystemComponent
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			// 套用效果
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

