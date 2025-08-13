// Copyright Luke


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(float XP)
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[1] 相當於 Level 1 的資訊
		// LevelUpInformation[2] 相當於 Level 2 的資訊
		if (LevelUpInformation.Num() - 1 <= Level)
		{
			return Level;
		}
		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
		Level++;
	}
	return Level;
}
