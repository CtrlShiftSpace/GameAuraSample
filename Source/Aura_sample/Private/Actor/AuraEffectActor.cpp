// Copyright Luke


#include "Actor/AuraEffectActor.h"
#include "Components/SphereComponent.h"
#include "Delegates/Delegate.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	// 設定為root節點
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	// 在root節點下
	Sphere->SetupAttachment(GetRootComponent());

}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// 檢查重疊的物件是否有 IAbilitySystemInterface 介面
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor)) 
	{
		//TODO: 之後要改變這段程式符合Gameplay的作法，暫時先用const_cast
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		
		// 因為AuraAttributeSet是const，因此不能在此pointer進行修改，以下這段是為教學使用，不建議用此作法
		UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.5);
		// 降低Mana值
		MutableAuraAttributeSet->SetMana(AuraAttributeSet->GetMana() - 25.5);
		Destroy();
	}
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
}


