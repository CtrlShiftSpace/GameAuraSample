// Copyright Luke


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

struct FInputAction;

AAuraPlayerController::AAuraPlayerController()
{
	// 當server更新值，發送到client端機制
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 避免AuraContext尚未指定，未指定時會回傳false並中斷
	check(AuraContext);

	// 取得local玩家的增強輸入
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// 確認是否成功取得
	if (Subsystem) 
	{
		// 新增mapping
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	// 是否顯示滑鼠
	bShowMouseCursor = true;
	// 預設滑鼠顯示模式
	DefaultMouseCursor = EMouseCursor::Default;

	// 遊戲與UI的輸入模式
	FInputModeGameAndUI InputModeData;
	// 設定相關滑鼠行為
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// CastCheck當轉換失敗時會觸發Assertion，並且停止執行
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	// 綁定 Pressed/Released/Held 呼叫事件
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) {
		return;
	}

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// 檢查室按下滑鼠左鍵
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	// 如果是點擊到敵人則觸發能力
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		const APawn* ControlledPawn = GetPawn();
		// 如果滑鼠跟隨時間小於設置的點擊時長，則認為是點擊動作
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			// 建立導航路徑
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				// 依序取出導航設定的座標
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}

				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints.Last();
					// 設定當前為自動移動
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) 
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	// 如果是點擊到敵人則觸發能力
	if (bTargeting)
	{
		
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		// 累加跟隨滑鼠的時間
		FollowTime += GetWorld()->GetDeltaSeconds();

		// 取得目前滑鼠的位置
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		// 計算滑鼠與角色的直線距離，並且normalize
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		// 找到最接近的 spline 上的點，並返回該點的 FVector 座標
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		// 取得角色在這個位置上應該移動的方向
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		// 計算路線到目標點的距離
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		// 如果已經離目標點在可接受的距離，就停止自動移動
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}
