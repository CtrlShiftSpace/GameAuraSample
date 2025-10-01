// Copyright Luke


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	// 取得 FScalableFloat 設定傷害的數值
	const int32 ElectrocuteDamage = Damage.GetValueAtLevel(Level);
	// 取得此等級能力的魔力值消耗
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	// 取得此等級能力的冷卻時間
	const float Cooldown = GetCooldown(Level);
	FString LightningNumText = FString("connecting with the target, repeatedly causing ");
	if (Level > 1)
	{
		LightningNumText = FString::Printf(TEXT("Propagating to %d additional targets nearby, causing "), FMath::Min(Level, MaxNumShockTargets -1));
	}
	return FString::Printf(TEXT(
		// 標題
		"<Title>ELECTROCUTE</>\n\n"

		// 技能等級與魔力消耗
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// 技能描述
		// LightningNumText
		"<Default>Emits a beam of lightning, %s </>"
		// Damage
		"<Damage>%d</> <Default>lightning damage with"
		" a chance to stun</>"),

		// 參數
		Level,
		ManaCost,
		Cooldown,
		*LightningNumText,
		ElectrocuteDamage);
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	// 取得 FScalableFloat 設定傷害的數值
	const int32 ElectrocuteDamage = Damage.GetValueAtLevel(Level);
	// 取得此等級能力的魔力值消耗
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	// 取得此等級能力的冷卻時間
	const float Cooldown = GetCooldown(Level);
	FString LightningNumText = FString("connecting with the target, repeatedly causing ");
	if (Level > 1)
	{
		LightningNumText = FString::Printf(TEXT("Propagating to %d additional targets nearby, causing "), FMath::Min(Level, MaxNumShockTargets -1));
	}
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
		// LightningNumText
		"<Default>Emits a beam of lightning, %s </>"
		// Damage
		"<Damage>%d</> <Default>lightning damage with"
		" a chance to stun</>"),

		// 參數
		Level,
		ManaCost,
		Cooldown,
		*LightningNumText,
		ElectrocuteDamage);
}
