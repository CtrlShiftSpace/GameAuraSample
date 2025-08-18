// Copyright Luke


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "GameplayAbilities/Public/GameplayEffect.h"
#include "Interaction/CombatInterface.h"
#include "Math/UnrealMathUtility.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 從 Source 與 Target 取得 Tag
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max(Intelligence, 0.f);

	int32 PlayerLevel = 1;
	// 確認是否有實作 CombatInterface，注意這邊在 Implements 是使用 UCombatInterface
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 60.f + 1.5f * Intelligence + PlayerLevel * 5;
}
