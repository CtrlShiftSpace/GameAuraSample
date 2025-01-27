// Copyright Luke


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

// 這是 raw struct 僅作內部處理，因此不需要加上 F ，也不需要加上USTRUCT() 或 GENERATED_BODY 
struct AuraDamageStatics
{
	// 處理Attribute Capture，描述了要捕獲的屬性是哪個
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
	}
};

// 建立一個靜態的Instance
static const AuraDamageStatics& DamageStatics()
{
	// 定義一個靜態變數，這個變數只會初始化一次，在程式的整個執行過程中都存在
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargeteASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargeteASC ? TargeteASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorDef,
		EvaluationParameters,
		Armor
	);
	Armor = FMath::Max<float>(0.f, Armor);
	// 執行計算公式
	++Armor;

	// 用來儲存修改屬性數值的相關訊息
	const FGameplayModifierEvaluatedData EvaluateData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	// 修改目標屬性
	OutExecutionOutput.AddOutputModifier(EvaluateData);
}
