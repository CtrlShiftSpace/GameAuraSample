// Copyright Luke


#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// 如果開啟(true)的話，每一幀會調用Tick()，這邊關閉(false)可以節省效能
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
