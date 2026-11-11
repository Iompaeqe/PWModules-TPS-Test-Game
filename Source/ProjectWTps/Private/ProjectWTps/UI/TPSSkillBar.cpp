// copyright mkurtt96

#include "ProjectWTPS/UI/TPSSkillBar.h"

#include "Components/PanelWidget.h"
#include "GAS/Abilities/PWAbilitySet.h"
#include "ProjectWTPS/Settings/GameSettings.h"
#include "ProjectWTPS/UI/TPSSkillButton.h"

void UTPSSkillBar::NativeConstruct()
{
	Super::NativeConstruct();

	const UPWAbilitySet* AbilitySet = UGameSettings::Get()->GetDefaultAbilitySet();
	if (!AbilitySet || !SkillButtonClass) return;

	ButtonPanel->ClearChildren();
	for (const FPWAbilitySetEntry& Entry : AbilitySet->Abilities)
	{
		if (!Entry.InputTag.IsValid()) continue;

		UTPSSkillButton* Button = CreateWidget<UTPSSkillButton>(GetOwningPlayer(), SkillButtonClass);
		ButtonPanel->AddChild(Button);
		Button->SetSkill(Entry.AbilityTag, Entry.InputTag);
	}
}
