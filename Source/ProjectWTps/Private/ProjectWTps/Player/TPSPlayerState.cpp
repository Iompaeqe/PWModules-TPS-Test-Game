// copyright mkurtt96


#include "ProjectWTPS/Player/TPSPlayerState.h"

#include "ProjectWTPS/AbilitySystem/AttributeSet_Core.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "Libraries/PWGasFunctions.h"
#include "ProjectWTPS/AbilitySystem/AttributeSet_Vitals.h"
#include "ProjectWTPS/Settings/GameSettings.h"
#include "TeamComponent.h"
#include "TimerManager.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"

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
	return UPWGasFunctions::ApplyEffects(GetASC(), SpellParams);
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

void ATPSPlayerState::HandleDeath()
{
	if (!HasAuthority() || bDying)
		return;

	// Death is reported from inside an effect being applied, so the pawn is removed on the next tick.
	bDying = true;
	ASC->CancelAllAbilities();
	GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::FinishDeath);
}

void ATPSPlayerState::FinishDeath()
{
	bDying = false;

	APawn* Pawn = GetPawn();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	if (!Pawn)
		return;

	APlayerController* Player = Cast<APlayerController>(Controller);
	if (!Player)
	{
		Pawn->Destroy();
		if (Controller)
			Controller->Destroy();
		return;
	}

	Player->UnPossess();
	Pawn->Destroy();

	ASC->SetNumericAttributeBase(UAttributeSet_Vitals::GetHealthAttribute(), ASVitals->GetHealthMax());
	ASC->SetNumericAttributeBase(UAttributeSet_Vitals::GetShieldAttribute(), ASVitals->GetShieldMax());
	ASC->SetNumericAttributeBase(UAttributeSet_Vitals::GetManaAttribute(), ASVitals->GetManaMax());
	ASC->SetNumericAttributeBase(UAttributeSet_Vitals::GetStaminaAttribute(), ASVitals->GetStaminaMax());

	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
		GameMode->RestartPlayer(Player);
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
