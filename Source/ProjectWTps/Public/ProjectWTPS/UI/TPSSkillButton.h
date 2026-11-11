// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GAS/Abilities/PWAbilityInfo.h"
#include "ProjectWTPS/UI/TPSUserWidget.h"
#include "TPSSkillButton.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROJECTWTPS_API UTPSSkillButton : public UTPSUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Skill")
	void SetSkill(const FGameplayTag& InAbilityTag, const FGameplayTag& InInputTag);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Skill")
	void OnSkillSet();
	UFUNCTION(BlueprintImplementableEvent, Category="Skill")
	void OnCooldownUpdated(float Remaining, float Duration);
	UFUNCTION(BlueprintImplementableEvent, Category="Skill")
	void OnActiveChanged(bool bIsActive);

	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FGameplayTag AbilityTag;
	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FGameplayTag InputTag;
	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FPWAbilityInfo AbilityInfo;
	UPROPERTY(BlueprintReadOnly, Category="Skill")
	FText KeyText;
	UPROPERTY(BlueprintReadOnly, Category="Skill")
	bool bAbilityActive = false;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UImage> Icon;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> KeyLabel;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> CooldownLabel;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UImage> CooldownBar;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UWidget> ActiveHighlight;

	UPROPERTY(EditDefaultsOnly, Category="Skill")
	FName CooldownParameter = "Progress";

private:
	UFUNCTION()
	void RefreshKeyText();

	FText FindKeyText() const;
	void SetCooldownProgress(float Progress) const;
	void SetAbilityActive(bool bIsActive);

	bool bOnCooldown = false;
};
