// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "PWAttributeSetBase.h"
#include "AttributeSet_Core.generated.h"


class ATPSPlayerState;

UCLASS()
class PROJECTWTPS_API UAttributeSet_Core : public UPWAttributeSetBase
{
	GENERATED_BODY()

public:
#pragma region Offense Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower, Category = "Offense") FGameplayAttributeData AttackPower; UFUNCTION() void OnRep_AttackPower(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AttackPower, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, AttackPower);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Offense") FGameplayAttributeData CriticalChance; UFUNCTION() void OnRep_CriticalChance(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalChance, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, CriticalChance);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalPower, Category = "Offense") FGameplayAttributeData CriticalPower; UFUNCTION() void OnRep_CriticalPower(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalPower, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, CriticalPower);
#pragma endregion
#pragma region Defense Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackResistance, Category = "Defense") FGameplayAttributeData AttackResistance; UFUNCTION() void OnRep_AttackResistance(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AttackResistance, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, AttackResistance);
#pragma endregion
#pragma region Utility Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Utility") FGameplayAttributeData MovementSpeed; UFUNCTION() void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MovementSpeed, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, MovementSpeed);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Utility") FGameplayAttributeData AttackSpeed; UFUNCTION() void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AttackSpeed, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, AttackSpeed);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CooldownReduction, Category = "Utility") FGameplayAttributeData CooldownReduction; UFUNCTION() void OnRep_CooldownReduction(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CooldownReduction, OldValue); } ATTRIBUTE_ACCESSORS(ThisClass, CooldownReduction);
#pragma endregion

protected:

public:
	ATPSPlayerState* PS;
	ATPSPlayerState* GetPS();

	
};
