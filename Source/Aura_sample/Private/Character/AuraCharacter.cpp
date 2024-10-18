// Copyright Luke


#include "Character/AuraCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// 建立SpringArm元件
	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>("CharacterSpringArm");
	CharacterSpringArm->bInheritPitch = false;
	CharacterSpringArm->bInheritRoll = false;
	CharacterSpringArm->bInheritYaw = false;
	CharacterSpringArm->SetupAttachment(GetCapsuleComponent(), FName("CharacterSpringArm"));

	// 建立攝影機
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>("CharacterCamera");
	CharacterCamera->SetupAttachment(CharacterSpringArm, FName("CharacterCamera"));

	// 設定角色移動面對方向也會跟著移動
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 取消預設的旋轉朝向
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 初始化Server的 Ability Actor Info
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 初始化Client的 Ability Actor Info
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	AttributeSet = AuraPlayerState->GetAttributeSet();
}

