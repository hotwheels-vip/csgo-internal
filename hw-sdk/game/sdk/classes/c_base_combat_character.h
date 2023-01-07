#pragma once
#include <iostream>

#include "../../../globals/includes/includes.h"

#include "c_base_animating.h"
#include "c_base_combat_weapon.h"
#include "c_handle.h"

struct c_base_attributable_item;

namespace sdk
{
	struct c_base_combat_character : public c_base_animating {
	public:
		NETVAR( active_weapon, std::uintptr_t, "CBaseCombatCharacter", "m_hActiveWeapon" );
		NETVAR( next_attack, float, "CBaseCombatCharacter", "m_hActiveWeapon" );
		NETVAR( my_weapons, std::uintptr_t, "CBaseCombatCharacter", "m_hMyWeapons" );
		NETVAR( my_wearables, std::uintptr_t, "CBaseCombatCharacter", "m_hMyWearables" );
	};
} // namespace sdk
