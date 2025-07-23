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
		const FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		// 將生成位置加入陣列
		SpawnLocations.Add(ChosenSpawnLocation);
		
		DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);
		// 產生 Debug 箭頭，會需要傳入箭頭起始座標
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);
		DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
		DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
	}

	return SpawnLocations;
}
