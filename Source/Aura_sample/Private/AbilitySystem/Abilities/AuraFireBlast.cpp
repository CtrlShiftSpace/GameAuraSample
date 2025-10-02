// Copyright Luke


#include "AbilitySystem/Abilities/AuraFireBlast.h"

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
