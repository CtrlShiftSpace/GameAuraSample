// Copyright Luke


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


APointCollection::APointCollection()
{
 	PrimaryActorTick.bCanEverTick = false;

	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	SetRootComponent(Pt_0);
	ImmutablePts.Add(Pt_0);

	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	Pt_1->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_1);

	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	Pt_2->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_2);

	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	Pt_3->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_3);

	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	Pt_4->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_4);

	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	Pt_5->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_5);

	Pt_6 = CreateDefaultSubobject<USceneComponent>("Pt_6");
	Pt_6->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_6);

	Pt_7 = CreateDefaultSubobject<USceneComponent>("Pt_7");
	Pt_7->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_7);

	Pt_8 = CreateDefaultSubobject<USceneComponent>("Pt_8");
	Pt_8->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_8);

	Pt_9 = CreateDefaultSubobject<USceneComponent>("Pt_9");
	Pt_9->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_9);

	Pt_10 = CreateDefaultSubobject<USceneComponent>("Pt_10");
	Pt_10->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_10);
	
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	// 檢查所需要的數量是否超出設定陣列範圍
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("Attempted to access ImmutablePts out of bounds."));

	TArray<USceneComponent*> ArrayCopy;
	for (USceneComponent* Pt : ImmutablePts)
	{
		// 如果陣列數量已經足夠，則回傳陣列
		if (ArrayCopy.Num() >= NumPoints)
		{
			return ArrayCopy;
		}
		// 確認不是根節點
		if (Pt != Pt_0)
		{
			// 設定點的位置與旋轉角度
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}

		// 取得點提升到地面上方500單位位置
		const FVector RaisedLocation = FVector(
			Pt->GetComponentLocation().X,
			Pt->GetComponentLocation().Y,
			GroundLocation.Z + 500.f
		);

		// 取得點降低到地面下方500單位位置
		const FVector LoweredLocation = FVector(
			Pt->GetComponentLocation().X,
			Pt->GetComponentLocation().Y,
			GroundLocation.Z - 500.f
		);

		// 透過兩點間的線段偵測地面的位置
		FHitResult HitResult;
		// 取得範圍內的角色，將其加入忽略陣列，避免偵測到非地面物件
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

		// 使用 Profile 進行線段偵測
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		// 取得偵測到地面的位置
		const FVector AdjustedLocation = FVector(
			Pt->GetComponentLocation().X,
			Pt->GetComponentLocation().Y,
			HitResult.ImpactPoint.Z
		);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		// 將點加入回傳陣列
		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

