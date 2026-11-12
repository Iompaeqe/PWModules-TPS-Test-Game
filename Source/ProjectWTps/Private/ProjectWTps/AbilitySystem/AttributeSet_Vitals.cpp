// copyright mkurtt96


#include "ProjectWTPS/AbilitySystem/AttributeSet_Vitals.h"

#include "FEffectProperties.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "ProjectWTPS/Player/TPSPlayerState.h"
#include "ProjectWTPS/Tags/ProjectWTpsTags.h"

UAttributeSet_Vitals::UAttributeSet_Vitals()
{
	TagsToAttributes.Add(PWTags::Attribute::Vital::Health, GetHealthAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::HealthMax, GetHealthMaxAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::HealthRegen, GetHealthRegenAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::Shield, GetShieldAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::ShieldMax, GetShieldMaxAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::ShieldRegen, GetShieldRegenAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::Mana, GetManaAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::ManaMax, GetManaMaxAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::ManaRegen, GetManaRegenAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::Stamina, GetStaminaAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::StaminaMax, GetStaminaMaxAttribute());
	TagsToAttributes.Add(PWTags::Attribute::Vital::StaminaRegen, GetStaminaRegenAttribute());

}

void UAttributeSet_Vitals::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vitals
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ShieldMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ShieldRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, StaminaMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, StaminaRegen, COND_None, REPNOTIFY_Always);
}

void UAttributeSet_Vitals::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttributes(Attribute, NewValue);
}

void UAttributeSet_Vitals::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// todo: if dead return
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetHealthMax()));
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetManaMax()));
	if (Data.EvaluatedData.Attribute == GetShieldAttribute())
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetShieldMax()));
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetStaminaMax()));
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
		HandleIncomingDamage(Props);
}

void UAttributeSet_Vitals::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UAttributeSet_Vitals::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
		NewValue = FMath::Clamp(NewValue, 0, GetHealthMax());
	if (Attribute == GetShieldAttribute())
		NewValue = FMath::Clamp(NewValue, 0, GetShieldMax());
	if (Attribute == GetManaAttribute())
		NewValue = FMath::Clamp(NewValue, 0, GetManaMax());
	if (Attribute == GetStaminaAttribute())
		NewValue = FMath::Clamp(NewValue, 0, GetStaminaMax());
}

void UAttributeSet_Vitals::HandleIncomingDamage(const FEffectProperties& Props)
{
	float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0);
	if (LocalIncomingDamage <= 0) return;

	float ShieldAbsorb = LocalIncomingDamage * GetShieldEffectiveness();
	float ActualAbsorb = FMath::Min(ShieldAbsorb, GetShield());
	
	float DamageToHealth = LocalIncomingDamage - ActualAbsorb;
	if (GetHealth() < DamageToHealth)
		DamageToHealth = GetHealth() + KINDA_SMALL_NUMBER; // Error margin
	
	const float NewShield = GetShield() - ActualAbsorb;
	SetShield(FMath::Clamp(NewShield, 0.0f, GetShieldMax()));
	const float NewHealth = GetHealth() - DamageToHealth;
	SetHealth(FMath::Clamp(NewHealth, 0.0f, GetHealthMax()));

	const bool bFatal = NewHealth <= 0;
	if (bFatal)
	{
		HandleDeath(Props);
	}
}

void UAttributeSet_Vitals::HandleDeath(const FEffectProperties& Props)
{
}

ATPSPlayerState* UAttributeSet_Vitals::GetPS()
{
	if (!PS)
		PS = Cast<ATPSPlayerState>(GetOwningActor());
	return PS;
}
