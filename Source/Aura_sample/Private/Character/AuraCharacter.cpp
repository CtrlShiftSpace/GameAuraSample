// Copyright Luke


#include "Character/AuraCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"

AAuraCharacter::AAuraCharacter()
{
	// 建立SpringArm元件
	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>("CharacterSpringArm");
	CharacterSpringArm->bInheritPitch = false;
	CharacterSpringArm->bInheritRoll = false;
	CharacterSpringArm->bInheritYaw = false;
	CharacterSpringArm->SetUsingAbsoluteRotation(true);
	CharacterSpringArm->bDoCollisionTest = false;
	CharacterSpringArm->SetupAttachment(GetCapsuleComponent(), FName("CharacterSpringArm"));

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	// 避免自動啟動特效
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	// 建立攝影機
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>("CharacterCamera");
	CharacterCamera->SetupAttachment(CharacterSpringArm, USpringArmComponent::SocketName);
	CharacterCamera->bUsePawnControlRotation = false;
	
	// 設定角色移動面對方向也會跟著移動
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 取消預設的旋轉朝向
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 初始化Server的 Ability Actor Info
	InitAbilityActorInfo();
	LoadProgress();

}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 初始化Client的 Ability Actor Info
	InitAbilityActorInfo();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		// 取得相機位置
		const FVector CameraLocation = CharacterCamera->GetComponentLocation();
		// 取得粒子效果位置
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		// 計算從粒子效果位置到相機位置的方向
		// 並將其轉換為旋轉角度確保粒子效果面向相機
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InPlayerLevel);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatus(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	// 取得 GameMode 並轉為 AuraGameMode 類別
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode )
	{
		// 從 AuraGameMode 類別取得用來存檔物件
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		// 如果不存在時就 return 
		if (SaveData == nullptr)
		{
			return;
		}
		// 將 CheckpointTag 存入 SaveData 的 PlayerStartTag 屬性中
		SaveData->PlayerStartTag = CheckpointTag;

		// 從 PlayerState 取得玩家相關資料並存入 SaveData 中
		if (AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())
		{
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->XP = AuraPlayerState->GetXP();
			SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
		}
		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

		// 標記不是第一次存檔
		SaveData->bFirstTimeLoadIn = false;

		// 只有 Server 端可以存檔
		if (!HasAuthority())
		{
			return;
		}

		// 取得 ASC
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		FForeachAbility SaveAbilityDelegate;
		// 清空之前的儲存能力陣列
		SaveData->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda(
			[this, AuraASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
			{
				// 先使用 Ability Spec 取得 Ability Tag
				const FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(AbilitySpec);
				// 再透過 Ability Tag 取得 Ability Info
				UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

				// 將要儲存的資訊存入 SavedAbility 結構中
				FSavedAbility SavedAbility;
				SavedAbility.GameplayAbility = Info.Ability;
				SavedAbility.AbilityLevel = AbilitySpec.Level;
				SavedAbility.AbilitySlot = AuraASC->GetSlotFromAbilityTag(AbilityTag);
				SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag);
				SavedAbility.AbilityTag = AbilityTag;
				SavedAbility.AbilityType = Info.AbilityType;

				// 將 SavedAbility 加入到 SaveData 的 SavedAbilities 陣列中
				SaveData->SavedAbilities.AddUnique(SavedAbility);
			}
		);
		// 使用 ForEachAbility 來遍歷所有能力並會放到 SavedAbilities 中
		AuraASC->ForEachAbility(SaveAbilityDelegate);
		
		// 呼叫 AuraGameMode 的 SaveInGameProgressData 方法來存檔
		AuraGameMode->SaveInGameProgressData(SaveData);
	}
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::LoadProgress()
{
	// 取得 GameMode 並轉為 AuraGameMode 類別
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode )
	{
		// 從 AuraGameMode 類別取得用來讀檔物件
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		// 如果不存在時就 return 
		if (SaveData == nullptr)
		{
			return;
		}
		
		// 如果是第一次載入遊戲，則初始化能力系統並添加角色能力
		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				// 從存檔資料中添加角色能力
				AuraASC->AddCharacterAbilitesFromSaveData(SaveData);
				AuraASC->UpdateAbilityStatus(SaveData->PlayerLevel);
			}
			
			// 取得存檔中的 PlayerState 資料並設定到目前的 PlayerState 中
			if (AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())
			{
				AuraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuraPlayerState->SetXP(SaveData->XP);
				AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void AAuraCharacter::OnRep_Stunned()
{
	// 當玩家暈眩/取消暈眩時，在 Client 端處理玩家的輸入
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		// 將所有需要封鎖的 Tag 加入 BlockedTags 容器中
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	// 呼叫UAuraAbilitySystemComponent的AbilityActorInfoSet方法，綁定delegate
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	// 執行HUD的InitOverlay
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController())) {
		// 之所以加 if 是為了處理多人玩家中其他玩家會是nullptr而不用執行以下內容，
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

