// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_SAMPLE_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// 設定預設值
	AAuraCharacterBase();
	
protected:
	// 當遊戲開始或物件產生時觸發
	virtual void BeginPlay() override;

	// 使用UProperty實現Reflection
	UPROPERTY(EditAnywhere, Category = "Combat");
	// TObjectPtr: TemplateClass wrapper around raw C++ Pointer
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
