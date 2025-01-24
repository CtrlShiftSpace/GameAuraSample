// Copyright Luke


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/Public/GameplayEffectTypes.h"
#include "GameplayAbilities/Public/GameplayEffect.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "Aura_sample/Aura_sample.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// 如果開啟(true)的話，每一幀會調用Tick()，這邊關閉(false)可以節省效能
	PrimaryActorTick.bCanEverTick = false;

	// 攝影機遮擋處理
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Projectile 碰撞
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// 建立 component 要使用 CreateDefaultSubobject
	// 帶入參數的SubobjectName的類型是FName，一般需要使用TEXT物件傳入，但仍可接受常規字串文字
	// 但使用TEXT()會因為使用寬字節會占用較多記憶體
	// 並對CPU有額外開銷且產生不必要的轉換，因此使用字串傳入
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// 將一個component附加到另一個component上
	// Inparent: 父component
	// InSocketName: 附加到父component 預設值為NAME_None
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	// 設定碰撞(無碰撞)
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 建立 Motion Warping
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::InitAbilityActorInfo()
{

}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	// 檢視是否為Server權限，不是就return
	if (!HasAuthority()) return;

	AuraASC->AddCharacterAbilites(StartupAbilities);
}
