// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;
class UAudioComponent;

UCLASS()
class AURA_SAMPLE_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	// DamageEffectSpecHandle 改用 DamageEffectParams 替代
	// UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	// FGameplayEffectSpecHandle DamageEffectSpecHandle;

	// 用來設定傷害效果的參數
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnHit();
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	// 檢查是否為有效的重疊事件
	bool IsValidOverlap(AActor* OtherActor);

	bool bHit = false;

private:

	// 存活在畫面中的時間
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	// 衝擊的粒子效果
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	// 衝擊造成的聲響
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	// 飛行過程的的聲音
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY();
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
};
