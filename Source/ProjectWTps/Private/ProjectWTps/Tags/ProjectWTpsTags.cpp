#include "ProjectWTPS/Tags/ProjectWTpsTags.h"


namespace PWTags::ASC::State
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "ASC.State");
	UE_DEFINE_GAMEPLAY_TAG(Moving, "ASC.State.Moving");
}

namespace PWTags::ASC::State::Casting
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "ASC.State.Casting");
	UE_DEFINE_GAMEPLAY_TAG(Channeling, "ASC.State.Casting.Channeling");
}



namespace PWTags::Attribute
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Attribute");
}

namespace PWTags::Attribute::Vital
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Attribute.Vital");
	UE_DEFINE_GAMEPLAY_TAG(Health, "Attribute.Vital.Health");
	UE_DEFINE_GAMEPLAY_TAG(HealthMax, "Attribute.Vital.HealthMax");
	UE_DEFINE_GAMEPLAY_TAG(HealthRegen, "Attribute.Vital.HealthRegen");
	UE_DEFINE_GAMEPLAY_TAG(Shield, "Attribute.Vital.Shield");
	UE_DEFINE_GAMEPLAY_TAG(ShieldMax, "Attribute.Vital.ShieldMax");
	UE_DEFINE_GAMEPLAY_TAG(ShieldRegen, "Attribute.Vital.ShieldRegen");
	UE_DEFINE_GAMEPLAY_TAG(Mana, "Attribute.Vital.Mana");
	UE_DEFINE_GAMEPLAY_TAG(ManaMax, "Attribute.Vital.ManaMax");
	UE_DEFINE_GAMEPLAY_TAG(ManaRegen, "Attribute.Vital.ManaRegen");
	UE_DEFINE_GAMEPLAY_TAG(Stamina, "Attribute.Vital.Stamina");
	UE_DEFINE_GAMEPLAY_TAG(StaminaMax, "Attribute.Vital.StaminaMax");
	UE_DEFINE_GAMEPLAY_TAG(StaminaRegen, "Attribute.Vital.StaminaRegen");
}

namespace PWTags::Attribute::Offense
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Attribute.Offense");
	UE_DEFINE_GAMEPLAY_TAG(AttackPower, "Attribute.Offense.AttackPower");
	UE_DEFINE_GAMEPLAY_TAG(CriticalChance, "Attribute.Offense.CriticalChance");
	UE_DEFINE_GAMEPLAY_TAG(CriticalPower, "Attribute.Offense.CriticalPower");
}

namespace PWTags::Attribute::Defense
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Attribute.Defense");
	UE_DEFINE_GAMEPLAY_TAG(AttackResistance, "Attribute.Defense.AttackResistance");
}

namespace PWTags::Attribute::Utility
{
	UE_DEFINE_GAMEPLAY_TAG(Root, "Attribute.Utility");
	UE_DEFINE_GAMEPLAY_TAG(MovementSpeed, "Attribute.Utility.MovementSpeed");
	UE_DEFINE_GAMEPLAY_TAG(AttackSpeed, "Attribute.Utility.AttackSpeed");
	UE_DEFINE_GAMEPLAY_TAG(CooldownReduction, "Attribute.Utility.CooldownReduction");
}
