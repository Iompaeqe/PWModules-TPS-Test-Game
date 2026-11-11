// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "PWInputConfig.h"
#include "GAS/Abilities/PWAbilitySet.h"
#include "Engine/DeveloperSettings.h"
#include "GameSettings.generated.h"
/**
 * 
 */

UCLASS(config=Game, defaultconfig)
class PROJECTWTPS_API UGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	static const UGameSettings* Get(){ return GetDefault<UGameSettings>(); }
	virtual FName GetCategoryName() const override;

	TArray<TSubclassOf<UGameplayEffect>> GetDefaultEffects() const;
	UPWAbilitySet* GetDefaultAbilitySet() const;
	UPWInputConfig* GetInputConfig() const;

protected:
	UPROPERTY(EditDefaultsOnly, config, Category="GAS", meta=(AllowedClasses="GameplayEffect"))
	TArray<TSoftClassPtr<UGameplayEffect>> DefaultEffects;
	UPROPERTY(EditDefaultsOnly, config, BlueprintReadOnly, Category="GAS")
	TSoftObjectPtr<UPWAbilitySet> DefaultAbilitySet;

	UPROPERTY(EditDefaultsOnly, config, BlueprintReadOnly, Category="Input")
	TSoftObjectPtr<UPWInputConfig> InputConfig;
};
