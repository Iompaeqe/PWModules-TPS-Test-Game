// copyright mkurtt96

#include "ProjectWTPS/UI/TPSSkillButton.h"

#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "PWInputConfig.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProjectWTPS/Settings/GameSettings.h"
#include "Settings/PWGASCoreSettings.h"

void UTPSSkillButton::SetSkill(const FGameplayTag& InAbilityTag, const FGameplayTag& InInputTag)
{
	AbilityTag = InAbilityTag;
	InputTag = InInputTag;
	AbilityInfo = UPWGASCoreSettings::Get()->GetAbilityByTag(AbilityTag);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer()))
		Subsystem->ControlMappingsRebuiltDelegate.AddUniqueDynamic(this, &ThisClass::RefreshKeyText);
	RefreshKeyText();

	if (Icon && AbilityInfo.IconMaterial)
		Icon->SetBrushFromMaterial(const_cast<UMaterialInstance*>(AbilityInfo.IconMaterial.Get()));
	if (CooldownLabel)
		CooldownLabel->SetText(FText::GetEmpty());
	SetCooldownProgress(0.f);
	SetAbilityActive(false);

	OnSkillSet();
}

void UTPSSkillButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const UPWAbilitySystemComponent* ASC = GetASC();
	if (!ASC || !AbilityTag.IsValid()) return;

	const FGameplayAbilitySpec* Spec = ASC->Data().GetSpecOfAbility(AbilityTag);
	const bool bIsActive = Spec && Spec->IsActive();
	if (bIsActive != bAbilityActive)
		SetAbilityActive(bIsActive);

	float Remaining = 0.f, Duration = 0.f;
	const bool bCooling = ASC->Cooldown().GetCooldownRemainingForAbilityTag(AbilityTag, Remaining, Duration) && Remaining > 0.f;
	if (!bCooling && !bOnCooldown) return;

	bOnCooldown = bCooling;
	if (!bCooling)
		Remaining = 0.f;

	if (CooldownLabel)
		CooldownLabel->SetText(bCooling ? FText::AsNumber(FMath::CeilToInt(Remaining)) : FText::GetEmpty());
	SetCooldownProgress(Duration > 0.f ? Remaining / Duration : 0.f);

	OnCooldownUpdated(Remaining, Duration);
}

void UTPSSkillButton::SetCooldownProgress(const float Progress) const
{
	if (!CooldownBar) return;

	if (UMaterialInstanceDynamic* Material = CooldownBar->GetDynamicMaterial())
		Material->SetScalarParameterValue(CooldownParameter, Progress);
}

void UTPSSkillButton::SetAbilityActive(const bool bIsActive)
{
	bAbilityActive = bIsActive;
	if (ActiveHighlight)
		ActiveHighlight->SetVisibility(bIsActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	OnActiveChanged(bIsActive);
}

void UTPSSkillButton::RefreshKeyText()
{
	KeyText = FindKeyText();
	if (KeyLabel)
		KeyLabel->SetText(KeyText);
}

FText UTPSSkillButton::FindKeyText() const
{
	UPWInputConfig* InputConfig = UGameSettings::Get()->GetInputConfig();
	const UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (!InputConfig || !Subsystem) return FText::GetEmpty();

	const UInputAction* Action = InputConfig->FindAbilityInputActionForTag(InputTag, false);
	if (!Action) return FText::GetEmpty();

	const TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(Action);
	return Keys.Num() > 0 ? Keys[0].GetDisplayName(false) : FText::GetEmpty();
}
