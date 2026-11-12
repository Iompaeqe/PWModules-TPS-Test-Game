// copyright mkurtt96

#include "ProjectWTPS/World/WorldDamageableActor.h"

#include "ProjectWTPS/AbilitySystem/AttributeSet_Vitals.h"
#include "World/PWDamageableComponent.h"

AWorldDamageableActor::AWorldDamageableActor()
{
	Damageable->DamageAttribute = UAttributeSet_Vitals::GetIncomingDamageAttribute();
}
