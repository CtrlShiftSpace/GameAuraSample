// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "Aura_sample/Aura_sample.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	/* Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

	// 是否位於此存檔位置
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	// 是否綁定重疊的事件
	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallback = true;
protected:
	// 接觸到其他物體的重疊函式
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	/* end Highlight Interface */

	// 設定要移動到的元件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
	
	// 提供可修改的 CustomDepthStencil 值
	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	// 玩家到達紀錄點時的事件
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	// 處理發光效果的函式
	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();

	// 用來顯示紀錄點位置的 Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
private:

	// 用來偵測接觸狀況的碰撞體
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
};
