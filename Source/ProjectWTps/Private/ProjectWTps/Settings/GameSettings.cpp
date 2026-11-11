// copyright mkurtt96


#include "ProjectWTPS/Settings/GameSettings.h"

FName UGameSettings::GetCategoryName() const
{
	return TEXT("Game"); 
}

TArray<TSubclassOf<UGameplayEffect>> UGameSettings::GetDefaultEffects() const
{
	TArray<TSubclassOf<UGameplayEffect>> Result;
	Result.Reserve(DefaultEffects.Num());

	for (const TSoftClassPtr<UGameplayEffect>& SoftEffect : DefaultEffects)
	{
		if (UClass* EffectClass = SoftEffect.LoadSynchronous())
		{
			Result.Add(EffectClass); // class type for MakeOutgoingSpec, etc.
		}
	}
	return Result;
}

UPWAbilitySet* UGameSettings::GetDefaultAbilitySet() const
{
	return DefaultAbilitySet.LoadSynchronous();
}

UPWInputConfig* UGameSettings::GetInputConfig() const
{
	return InputConfig.LoadSynchronous();
}
