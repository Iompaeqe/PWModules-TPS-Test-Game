// copyright mkurtt96


#include "ProjectWTPS/AbilitySystem/AttributeSet_Core.h"

#include "GameplayEffectExtension.h"
#include "ProjectWTPS/Tags/ProjectWTpsTags.h"
#include "Net/UnrealNetwork.h"
#include "ProjectWTPS/Player/TPSPlayerState.h"

PW_REPLICATE_ATTRIBUTES(UAttributeSet_Core)

ATPSPlayerState* UAttributeSet_Core::GetPS()
{
	if (!PS)
		PS = Cast<ATPSPlayerState>(GetOwningActor());
	return PS;
}
