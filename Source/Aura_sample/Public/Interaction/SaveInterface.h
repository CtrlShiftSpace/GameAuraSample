// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_SAMPLE_API ISaveInterface
{
	GENERATED_BODY()

public:

	// 是否需要讀取位置與旋轉資訊
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldLoadTransform();

	// 讀取 Actor 狀態
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadActor();
};
