// Copyright Luke


#include "AbilitySystem/Abilities/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	// 取得角色所面對方向的 Vector
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	// 取得角色Actor的位置
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	// 依照Z軸旋轉計算出左邊邊界的旋轉角度
	const FVector LeftOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	// 產生 Debug 箭頭，會需要傳入箭頭起始座標
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);

	// 依照Z軸旋轉計算出右邊邊界的旋轉角度
	const FVector RightOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	// 產生 Debug 箭頭，會需要傳入箭頭起始座標
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);

	// 暫時回傳空陣列
	return TArray<FVector>();
}
