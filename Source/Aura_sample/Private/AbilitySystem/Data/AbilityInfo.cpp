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
		UE_LOG(LogAura, Error, TEXT("�b AbilityInfo [%s] ���䤣�즹 AbilityTag [%s]"), *GetNameSafe(this), *AbilityTag.ToString());
	}

	return FAuraAbilityInfo();
}
