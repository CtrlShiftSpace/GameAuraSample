// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;


protected:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CharacterSpringArm;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CharacterCamera;

private:
	void InitAbilityActorInfo();

};
