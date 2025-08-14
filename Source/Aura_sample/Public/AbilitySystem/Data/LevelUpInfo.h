// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	// 等級經驗值
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;
	
	// 升級後的屬性點獎勵
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	// 升級後的技能點獎勵
    UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	// 藉由經驗值取得等級
	int32 FindLevelForXP(float XP) const;

};
