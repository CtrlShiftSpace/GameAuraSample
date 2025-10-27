// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()

public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	
	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	/* end Highlight Interface */

	// 前往的目標地圖
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	// 目標地圖開始的 PlayerStartTag
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
