// Copyright Luke


#include "AbilitySystem/Data/AttributeInfo.h"

FauraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound /*= false*/) const
{
	for (const FauraAttributeInfo& Info : AttributeInformation)
	{
		// 檢查是否符合
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	// 當未找到符合內容時，是否顯示錯誤訊息
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FauraAttributeInfo();
}
