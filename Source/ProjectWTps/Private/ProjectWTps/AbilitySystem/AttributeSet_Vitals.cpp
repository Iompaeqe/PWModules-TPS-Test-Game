// copyright mkurtt96


#include "ProjectWTPS/AbilitySystem/AttributeSet_Vitals.h"

#include "PWEffectProperties.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "ProjectWTPS/Player/TPSPlayerState.h"
#include "ProjectWTPS/Tags/ProjectWTpsTags.h"

PW_REPLICATE_ATTRIBUTES(UAttributeSet_Vitals)

UAttributeSet_Vitals::UAttributeSet_Vitals()
{
	AddClamped(GetHealthAttribute(), GetHealthMaxAttribute());
	AddClamped(GetShieldAttribute(), GetShieldMaxAttribute());
	AddClamped(GetManaAttribute(), GetManaMaxAttribute());
	AddClamped(GetStaminaAttribute(), GetStaminaMaxAttribute());
}

void UAttributeSet_Vitals::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FPWEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
		HandleIncomingDamage(Props);
}

void UAttributeSet_Vitals::HandleIncomingDamage(const FPWEffectProperties& Props)
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

void UAttributeSet_Vitals::HandleDeath(const FPWEffectProperties& Props)
{
	if (ATPSPlayerState* State = GetPS())
		State->HandleDeath();
}

ATPSPlayerState* UAttributeSet_Vitals::GetPS()
{
	if (!PS)
		PS = Cast<ATPSPlayerState>(GetOwningActor());
	return PS;
}
