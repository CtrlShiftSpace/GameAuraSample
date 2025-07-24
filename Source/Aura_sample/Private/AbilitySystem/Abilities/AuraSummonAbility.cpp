// Copyright Luke


#include "AbilitySystem/Abilities/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	// 取得角色所面對方向的 Vector
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	// 取得角色Actor的位置
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	// 每個生成敵人的間隔角度
	const float DeltaSpread = SpawnSpread / NumMinions;

	// 依照Z軸旋轉計算出左邊邊界的旋轉角度
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	// 依照Z軸旋轉計算出右邊邊界的旋轉角度
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	// 生成敵人位置的陣列
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		// 依照設定的角度與敵人數，平均分佈生成敵人方向
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		
		// 檢查生成位置上下所接觸到的平面位置，將結果存入Hit變數
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint; // 如果有碰撞，使用碰撞平面作為生成位置
		}
		
		// 將生成位置加入陣列
		SpawnLocations.Add(ChosenSpawnLocation);
	}

	return SpawnLocations;
}
