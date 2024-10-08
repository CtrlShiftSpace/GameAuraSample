// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

// 要加這行需在專案名.Build.cs 檔案中的PublicDependencyModuleNames.AddRange 加上 "EnhancedInput"
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// 建構函式
	AAuraPlayerController();
protected:
	// 覆寫BeginPlay方法
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "Input");
	TObjectPtr<UInputMappingContext> AuraContext;
};
