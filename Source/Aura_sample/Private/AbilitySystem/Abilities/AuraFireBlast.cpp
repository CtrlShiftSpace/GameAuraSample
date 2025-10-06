// Copyright Luke


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	// 取得 FScalableFloat 設定傷害的數值
	const int32 FireDamage = Damage.GetValueAtLevel(Level);
	// 取得此等級能力的魔力值消耗
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	// 取得此等級能力的冷卻時間
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		// 標題
		"<Title>FIRE BLAST</>\n\n"

		// 技能等級與魔力消耗
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// 技能描述
		// Number of Fire Balls
		"<Default>Launches %d </>"
		"<Default>fire balls in all direction, each coming back and </>"
		"<Default>exploding upon return, causing </>"
		// Damage
		"<Damage>%d</> <Default> radial fire damage with"
		" a chance to burn</>"),

		// 參數
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		FireDamage);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	// 取得 FScalableFloat 設定傷害的數值
	const int32 FireDamage = Damage.GetValueAtLevel(Level);
	// 取得此等級能力的魔力值消耗
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	// 取得此等級能力的冷卻時間
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		// 標題
		"<Title>NEXT LEVEL</>\n\n"

		// 技能等級與魔力消耗
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// 技能描述
		// Number of Fire Balls
		"<Default>Launches %d </>"
		"<Default>fire balls in all direction, each coming back and </>"
		"<Default>exploding upon return, causing </>"
		// Damage
		"<Damage>%d</> <Default> radial fire damage with"
		" a chance to burn</>"),

		// 參數
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		FireDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	// 檢查是否為 Server 呼叫
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) TArray<AAuraFireBall*>();
	// 用來存放生成的火球陣列
	TArray<AAuraFireBall*> FireBalls;
	// 取得角色面向的方向向量
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	// 取得角色所在位置
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	// 取得均勻分佈的旋轉陣列
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		// 設定生成位置與旋轉
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		// 透過延遲生成火球方式，可以在正式完成生成之前修改屬性
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);

		// 設定火球的傷害數值與效果參數
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		// 設定火球返回的目標角色
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		
		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		
		// 加入到陣列中
		FireBalls.Add(FireBall);

		// 正式完成生成
		FireBall->FinishSpawning(SpawnTransform);
	}

	return FireBalls;
}
