// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWAttributeSetBase.h"
#include "AttributeSet_Vitals.generated.h"

class ATPSPlayerState;
/**
 * 
 */
UCLASS()
class PROJECTWTPS_API UAttributeSet_Vitals : public UPWAttributeSetBase
{
	GENERATED_BODY()

public:
	UAttributeSet_Vitals();
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

#pragma region Vital Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital") FGameplayAttributeData Health; UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, Health);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthMax, Category = "Vital") FGameplayAttributeData HealthMax; UFUNCTION() void OnRep_HealthMax(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthMax, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, HealthMax);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Vital") FGameplayAttributeData HealthRegen; UFUNCTION() void OnRep_HealthRegen(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthRegen, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, HealthRegen);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Vital") FGameplayAttributeData Shield; UFUNCTION() void OnRep_Shield(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Shield, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, Shield);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShieldMax, Category = "Vital") FGameplayAttributeData ShieldMax; UFUNCTION() void OnRep_ShieldMax(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ShieldMax, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, ShieldMax);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShieldRegen, Category = "Vital") FGameplayAttributeData ShieldRegen; UFUNCTION() void OnRep_ShieldRegen(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ShieldRegen, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, ShieldRegen);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShieldEffectiveness, Category = "Vital") FGameplayAttributeData ShieldEffectiveness; UFUNCTION() void OnRep_ShieldEffectiveness(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ShieldEffectiveness, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, ShieldEffectiveness);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital") FGameplayAttributeData Mana; UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, Mana);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaMax, Category = "Vital") FGameplayAttributeData ManaMax; UFUNCTION() void OnRep_ManaMax(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaMax, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, ManaMax);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegen, Category = "Vital") FGameplayAttributeData ManaRegen; UFUNCTION() void OnRep_ManaRegen(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaRegen, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, ManaRegen);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Vital") FGameplayAttributeData Stamina; UFUNCTION() void OnRep_Stamina(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Stamina, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, Stamina);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaMax, Category = "Vital") FGameplayAttributeData StaminaMax; UFUNCTION() void OnRep_StaminaMax(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, StaminaMax, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, StaminaMax);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRegen, Category = "Vital") FGameplayAttributeData StaminaRegen; UFUNCTION() void OnRep_StaminaRegen(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, StaminaRegen, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, StaminaRegen);
#pragma endregion
#pragma region Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Meta") FGameplayAttributeData IncomingDamage; ATTRIBUTE_ACCESSORS(ThisClass, IncomingDamage);
#pragma endregion

protected:
	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleDeath(const FEffectProperties& Props);

public:
	ATPSPlayerState* PS;
	ATPSPlayerState* GetPS();

	
};
