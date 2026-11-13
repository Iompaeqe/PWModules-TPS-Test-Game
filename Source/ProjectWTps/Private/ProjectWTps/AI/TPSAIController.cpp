// copyright mkurtt96

#include "ProjectWTPS/AI/TPSAIController.h"

#include "AbilitySystemGlobals.h"
#include "EngineUtils.h"
#include "ProjectWTps.h"
#include "TeamFunctions.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "GAS/ASC/PWASC_DataManagement.h"
#include "Engine/Engine.h"
#include "ProjectWTPS/AbilitySystem/AttributeSet_Vitals.h"

ATPSAIController::ATPSAIController()
{
	// The ability system, attributes and team live on the player state, so the AI needs one too.
	bWantsPlayerState = true;
}

void ATPSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InPawn->bUseControllerRotationYaw = true;
	GetWorldTimerManager().SetTimer(CastTimer, this, &ThisClass::CastNext, CastInterval, true);
}

void ATPSAIController::OnUnPossess()
{
	GetWorldTimerManager().ClearTimer(CastTimer);
	Super::OnUnPossess();
}

void ATPSAIController::CastNext()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
		return;

	APawn* Target = FindTarget();
	if (!Target)
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}
	SetFocus(Target);

	UPWAbilitySystemComponent* ASC = Cast<UPWAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MyPawn));
	if (!ASC)
		return;

	TArray<FGameplayTag> CastTags = AbilityTags;
	if (CastTags.IsEmpty())
		for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
			if (const FGameplayTag Tag = FPWASC_DataManagement::GetAbilityFromSpec(Spec); Tag.IsValid())
				CastTags.AddUnique(Tag);
	if (CastTags.IsEmpty())
		return;

	const FGameplayTag Tag = CastTags[NextAbility++ % CastTags.Num()];
	const bool bStarted = ASC->BP_ActivateAbility(Tag);

	const FString Message = FString::Printf(TEXT("AI %s cast %s: %s | health %.0f / %.0f"), *MyPawn->GetName(), *Tag.ToString(), bStarted ? TEXT("started") : TEXT("did not start"),
		ASC->GetNumericAttribute(UAttributeSet_Vitals::GetHealthAttribute()), ASC->GetNumericAttribute(UAttributeSet_Vitals::GetHealthMaxAttribute()));
	UE_LOG(LogProjectWTps, Log, TEXT("%s"), *Message);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(static_cast<int32>(GetUniqueID()), CastInterval, FColor::Orange, Message);
}

APawn* ATPSAIController::FindTarget() const
{
	const APawn* MyPawn = GetPawn();
	APawn* Best = nullptr;
	float BestDistance = TargetRange * TargetRange;

	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		APawn* Other = *It;
		if (Other == MyPawn || !UTeamFunctions::AreEnemies(MyPawn, Other))
			continue;

		const float Distance = FVector::DistSquared(MyPawn->GetActorLocation(), Other->GetActorLocation());
		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			Best = Other;
		}
	}
	return Best;
}
