// Copyright Luke


#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura_sample/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("在 AbilityInfo [%s] 中找不到此 AbilityTag [%s]"), *GetNameSafe(this), *AbilityTag.ToString());
	}

	return FAuraAbilityInfo();
}
