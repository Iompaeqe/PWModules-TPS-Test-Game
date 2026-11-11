// copyright mkurtt96


#include "ProjectWTPS/Player/TPSPlayerState.h"

#include "ProjectWTPS/AbilitySystem/AttributeSet_Core.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "Libraries/GASCoreFunctions.h"
#include "ProjectWTPS/AbilitySystem/AttributeSet_Vitals.h"
#include "ProjectWTPS/Settings/GameSettings.h"
#include "TeamComponent.h"

ATPSPlayerState::ATPSPlayerState()
{
	ASC = CreateDefaultSubobject<UPWAbilitySystemComponent>("AbilitySystemComponent");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ASCore = CreateDefaultSubobject<UAttributeSet_Core>("AttributeSet Core");
	ASVitals = CreateDefaultSubobject<UAttributeSet_Vitals>("AttributeSet Vitals");

	Team = CreateDefaultSubobject<UTeamComponent>("Team");
	Team->SetIsReplicated(true);

	SetNetUpdateFrequency(100);
}

bool ATPSPlayerState::ApplyEffects_Implementation(class USpellParams* SpellParams)
{
	return UGASCoreFunctions::ApplyEffects(GetASC(), SpellParams);
}

UObject* ATPSPlayerState::GetEffectReceiver_Implementation()
{
	return this;
}

void ATPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ATPSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ASC->InitAbilityActorInfo(this, GetPawn());

	if (HasAuthority())
	{
		Team->TeamId = GetUniqueID();

		const UGameSettings* GameSettings = UGameSettings::Get();
		GetASC()->Abilities().AddAbilitySet(GameSettings->GetDefaultAbilitySet());

		for (TSubclassOf<UGameplayEffect> GE : GameSettings->GetDefaultEffects())
		{
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle SpecHandle = GetASC()->MakeOutgoingSpec(GE, 1, ContextHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

UPWAbilitySystemComponent* ATPSPlayerState::GetASC() const
{
	return ASC;
}

UAttributeSet_Core* ATPSPlayerState::GetASCore() const
{
	return ASCore;
}
