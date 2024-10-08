// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

// 要加這行需在專案名.Build.cs 檔案中的PublicDependencyModuleNames.AddRange 加上 "EnhancedInput"
class UInputMappingContext;

class UInputAction;
struct FInputActionValue;

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
	virtual void SetupInputComponent() override;

private:
	// 將下列的變數能夠進行反射(reflect)
	UPROPERTY(EditAnywhere, Category = "Input")
	// 輸入Mapping
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	

	void Move(const FInputActionValue& InputActionValue);

};
