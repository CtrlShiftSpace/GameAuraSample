// Copyright Luke


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 FireDamage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
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
	const int32 FireDamage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
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
