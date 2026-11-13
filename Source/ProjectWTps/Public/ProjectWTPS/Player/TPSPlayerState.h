// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Ability/Affectable.h"
#include "Ability/EffectReceiver.h"
#include "GameFramework/PlayerState.h"
#include "TPSPlayerState.generated.h"

class UAttributeSet_Vitals;
class ATPSPlayerState;
struct FPWEffectProperties;
class UAttributeSet_Core;
class UPWAbilitySystemComponent;
class UTeamComponent;


UCLASS()
class PROJECTWTPS_API ATPSPlayerState : public APlayerState, public IAbilitySystemInterface, public IAffectable, public IEffectReceiver
{
	GENERATED_BODY()

public:
	ATPSPlayerState();
	//~IEffectable
	virtual bool ApplyEffects_Implementation(class USpellParams* SpellParams) override;
	//~IPWEffectReceiver
	virtual UObject* GetEffectReceiver_Implementation() override;	
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPWAbilitySystemComponent> ASC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeSet_Core> ASCore;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeSet_Vitals> ASVitals;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTeamComponent> Team;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UPWAbilitySystemComponent* GetASC() const;
	virtual UAttributeSet_Core* GetASCore() const;

	// Test-bed death: an AI is removed, a player respawns with full vitals.
	void HandleDeath();

private:
	void FinishDeath();
	bool bDying = false;

	
};
