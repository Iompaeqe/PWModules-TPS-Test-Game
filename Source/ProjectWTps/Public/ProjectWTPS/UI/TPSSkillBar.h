// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "ProjectWTPS/UI/TPSUserWidget.h"
#include "TPSSkillBar.generated.h"

class UPanelWidget;
class UTPSSkillButton;

UCLASS()
class PROJECTWTPS_API UTPSSkillBar : public UTPSUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category="Skill")
	TSubclassOf<UTPSSkillButton> SkillButtonClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPanelWidget> ButtonPanel;
};
