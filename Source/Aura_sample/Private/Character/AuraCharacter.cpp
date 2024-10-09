// Copyright Luke


#include "Character/AuraCharacter.h"
#include "Components/CapsuleComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// 建立SpringArm元件
	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>("CharacterSpringArm");
	CharacterSpringArm->SetupAttachment(GetCapsuleComponent(), FName("CharacterSpringArm"));

	// 建立攝影機
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>("CharacterCamera");
	CharacterCamera->SetupAttachment(CharacterSpringArm, FName("CharacterCamera"));

}


