// Copyright Luke


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	// 取得 FScalableFloat 設定傷害的數值
	const int32 FireDamage = Damage.GetValueAtLevel(Level);
	// 取得此等級能力的魔力值消耗
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	// 取得此等級能力的冷卻時間
	const float Cooldown = GetCooldown(Level);
	FString BoltNumText = FString("a bolt");
	if (Level > 1)
	{
		BoltNumText = FString::Printf(TEXT("%d bolts"), FMath::Min(Level, NumProjectiles));
	}
	return FString::Printf(TEXT(
		// 標題
		"<Title>FIRE BOLT</>\n\n"

		// 技能等級與魔力消耗
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// 技能描述
		// BoltNumText
		"<Default>Launches %s of fire, "
		"exploding on impact and dealing: </>"
		// Damage
		"<Damage>%d</> <Default>fire damage with"
		" a chance to burn</>"),

		// 參數
		Level,
		ManaCost,
		Cooldown,
		*BoltNumText,
		FireDamage);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	// 取得 FScalableFloat 設定傷害的數值
	const int32 FireDamage = Damage.GetValueAtLevel(Level);
	// 取得此等級能力的魔力值消耗
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	// 取得此等級能力的冷卻時間
	const float Cooldown = GetCooldown(Level);
	FString BoltNumText = FString::Printf(TEXT("%d bolts"), FMath::Min(Level, NumProjectiles));
	return FString::Printf(TEXT(
		// 標題
		"<Title>NEXT LEVEL:</>\n\n"

		// 技能等級與魔力消耗
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// 技能描述
		// BoltNumText
		"<Default>Launches %s of fire, "
		"exploding on impact and dealing: </>"
		// Damage
		"<Damage>%d</> <Default>fire damage with"
		" a chance to burn</>"),

		// 參數
		Level,
		ManaCost,
		Cooldown,
		*BoltNumText,
		FireDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	// 檢查是否為 Server 呼叫
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	// 發射位置與目標位置的方向
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	// 發射方向
	const FVector Forward = Rotation.Vector();
	// 限制發射數量
	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	// 平均分配方向
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		// 生成位置
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// Set the Projectile Rotation
		SpawnTransform.SetRotation(Rot.Quaternion());

		// 生成投擲物件
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			// 注意這邊的 HomingTargetComponent 為 WeakPtr ，避免敵人死亡時仍被keep住
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			// 如果是點擊場景中物件，則在投擲物本身建立一個 SceneComponent 作為 HomingTargetComponent，當移除時會跟著 GC 處理掉
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
