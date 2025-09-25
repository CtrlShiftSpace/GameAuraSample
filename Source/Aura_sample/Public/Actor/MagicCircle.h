// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class AURA_SAMPLE_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// 顯示魔法陣元件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

};
