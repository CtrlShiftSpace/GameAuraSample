// Copyright Luke


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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
	// 左邊最大扇形方向
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2, FVector::UpVector);

	const FVector Start = SocketLocation + FVector(0, 0, 10);
	// 生成投擲物數量
	// NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	if (NumProjectiles > 1)
	{
		// 為了確保發射物最後一次執行要在最右端的位置，因此實際上角度需要除以 (投擲物數量-1)
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Direction * 75.f, 5.f, FLinearColor::Green, 120.f, 3.f);
		}
	}
	else
	{
		// 單一投擲物
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Forward * 75.f, 5.f, FLinearColor::Green, 120.f, 3.f);
	}

}
