// Copyright Luke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_SAMPLE_API ACheckpoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	// 是否位於此存檔位置
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;
protected:
	// 接觸到其他物體的重疊函式
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	// 玩家到達紀錄點時的事件
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	// 處理發光效果的函式
	void HandleGlowEffects();
	
private:
	// 用來顯示紀錄點位置的 Mesh
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	// 用來偵測接觸狀況的碰撞體
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
};
