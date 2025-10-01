// Copyright Luke


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	// 取得 FScalableFloat 設定傷害的數值
	const int32 ShardsDamage = Damage.GetValueAtLevel(Level);
	// 取得此等級能力的魔力值消耗
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	// 取得此等級能力的冷卻時間
	const float Cooldown = GetCooldown(Level);
	FString ShardsNumText = FString("Summon a shard of arcane energy, causing radial arcane damage of ");
	if (Level > 1)
	{
		ShardsNumText = FString::Printf(TEXT("Summon %d shards of additional targets nearby, causing radial arcane damage of "), FMath::Min(Level, MaxNumShards));
	}
	return FString::Printf(TEXT(
		// 標題
		"<Title>ARCANE SHARDS</>\n\n"

		// 技能等級與魔力消耗
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// 技能描述
		// ShardsNumText
		"<Default> %s </>"
		// Damage
		"<Damage>%d</> <Default> at the shard origin.</>"),

		// 參數
		Level,
		ManaCost,
		Cooldown,
		*ShardsNumText,
		ShardsDamage);
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	// 取得 FScalableFloat 設定傷害的數值
	const int32 ShardsDamage = Damage.GetValueAtLevel(Level);
	// 取得此等級能力的魔力值消耗
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	// 取得此等級能力的冷卻時間
	const float Cooldown = GetCooldown(Level);
	FString ShardsNumText = FString("Summon a shard of arcane energy, causing radial arcane damage of ");
	if (Level > 1)
	{
		ShardsNumText = FString::Printf(TEXT("Summon %d shards of additional targets nearby, causing radial arcane damage of "), FMath::Min(Level, MaxNumShards));
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
		// ShardsNumText
		"<Default> %s </>"
		// Damage
		"<Damage>%d</> <Default> at the shard origin.</>"),

		// 參數
		Level,
		ManaCost,
		Cooldown,
		*ShardsNumText,
		ShardsDamage);
}
