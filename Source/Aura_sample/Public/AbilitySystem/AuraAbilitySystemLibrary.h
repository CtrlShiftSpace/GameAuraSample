// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	// 取得 Overlay Widget Controller
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
};
