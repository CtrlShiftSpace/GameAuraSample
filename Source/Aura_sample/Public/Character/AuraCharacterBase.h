// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_SAMPLE_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// 設定預設值
	AAuraCharacterBase();
	// 覆寫介面的GetAbilitySystemComponent
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
protected:
	// 當遊戲開始或物件產生時觸發
	virtual void BeginPlay() override;

	// 使用UProperty實現Reflection
	UPROPERTY(EditAnywhere, Category = "Combat");
	// TObjectPtr: TemplateClass wrapper around raw C++ Pointer
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	// 提供子類別繼承
	virtual void InitAbilityActorInfo();

};
