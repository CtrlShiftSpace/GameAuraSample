// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAbilityInfo 
{
	GENERATED_BODY()

	// 用來識別能力的 Tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	// 對應輸入的Tag(由於輸入設定可能改變，不需要 EditDefaultsOnly 設定)
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	// 能力狀態的 Tag
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	// 判斷冷卻狀態的 Tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	// 顯示能力圖示
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	// 顯示能力背景
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
};


/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// 儲存能力相關資訊陣列
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FAuraAbilityInfo> AbilityInformation;

	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;

};
