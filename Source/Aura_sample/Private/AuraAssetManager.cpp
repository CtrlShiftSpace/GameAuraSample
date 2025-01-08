// Copyright Luke


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "GameplayAbilities/Public/AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	// 使用Target Data前要先呼叫此function(5.3版後因為已經由GameplayAbilitiesModule.cpp自動呼叫，可以此處也可省略)
	UAbilitySystemGlobals::Get().InitGlobalData();
}
