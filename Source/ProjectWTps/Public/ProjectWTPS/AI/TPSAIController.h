// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
#include "TPSAIController.generated.h"

// Test caster: focuses the nearest enemy pawn and casts its abilities in turn.
UCLASS()
class PROJECTWTPS_API ATPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// Seconds between casts.
	UPROPERTY(EditDefaultsOnly, Category="Casting", meta=(ClampMin="0.1"))
	float CastInterval = 3.f;

	// Abilities cast in turn; empty uses every ability the pawn was granted.
	UPROPERTY(EditDefaultsOnly, Category="Casting", meta=(GameplayTagFilter="Ability.Skill"))
	TArray<FGameplayTag> AbilityTags;

	// Enemies further away than this are ignored.
	UPROPERTY(EditDefaultsOnly, Category="Casting")
	float TargetRange = 3000.f;

private:
	void CastNext();
	APawn* FindTarget() const;

	FTimerHandle CastTimer;
	int32 NextAbility = 0;
};
