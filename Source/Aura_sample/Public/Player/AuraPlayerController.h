// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class AMagicCircle;
class UNiagaraSystem;
// 要加這行需在專案名.Build.cs 檔案中的PublicDependencyModuleNames.AddRange 加上 "EnhancedInput"
class UInputMappingContext;

class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;

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
	virtual void PlayerTick(float DeltaTime);

	// Remote Procedure Call
	// 此函式會由server端觸發，但只在特定client端上執行
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	// 顯示或隱藏魔法陣
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
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

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);
	
	void CursorTrace();
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	// 魔法陣類別
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	// 魔法陣物件指標
	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	// 更新魔法陣位置
	void UpdateMagicCircleLocation();
};
