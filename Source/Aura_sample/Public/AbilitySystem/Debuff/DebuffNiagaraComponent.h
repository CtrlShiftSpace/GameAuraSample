// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UDebuffNiagaraComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;

protected:
	virtual void BeginPlay() override;
	// 當 DebuffTag 變動時觸發
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	// 當擁有者死亡時觸發
	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
