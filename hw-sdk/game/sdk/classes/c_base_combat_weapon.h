#pragma once
#include <iostream>

#include "../../../globals/includes/includes.h"
#include "../structs/weapon_data_t.h"
#include "c_base_entity.h"

namespace sdk
{
	struct c_base_combat_weapon : public c_base_entity {
	public:
		NETVAR( next_primary_attack, float, "CBaseCombatWeapon", "m_flNextPrimaryAttack" );
		NETVAR( next_secondary_attack, float, "CBaseCombatWeapon", "m_flNextSecondaryAttack" );
		NETVAR( clip_mag, std::int32_t, "CBaseCombatWeapon", "m_iClip1" );
		NETVAR( clip_reserve, std::int32_t, "CBaseCombatWeapon", "m_iPrimaryReserveAmmoCount" );
		NETVAR( view_model_index, std::int32_t, "CBaseCombatWeapon", "m_iViewModelIndex" );
		NETVAR( world_model_index, std::int32_t, "CBaseCombatWeapon", "m_iWorldModelIndex" );
		NETVAR( world_dropped_model_index, std::int32_t, "CBaseCombatWeapon", "m_iWorldDroppedModelIndex" );
		NETVAR( postpone_fire_ready_time, float, "CBaseCombatWeapon", "m_flPostponeFireReadyTime" );
		NETVAR( world_view_model, std::uintptr_t, "CBaseCombatWeapon", "m_hWeaponWorldModel" );

		NETVAR( definition_index, short, "CBaseAttributableItem", "m_iItemDefinitionIndex" );

		[[nodiscard]] sdk::weapon_data_t* get_weapon_data( )
		{
			return virtual_func::call< sdk::weapon_data_t* >( this, 461 );
		}

		float inaccuracy( )
		{
			return virtual_func::call< float >( this, 483 );
		}

		float spread( )
		{
			return virtual_func::call< float >( this, 453 );
		}

		math::vec3 spread_vector( int seed, float inaccuracy, float spread )
		{
			weapon_data_t* wep_info;
			float r1, r2, r3, r4, s1, c1, s2, c2;

			wep_info = get_weapon_data( );
			if ( !wep_info || !wep_info->bullets )
				return { };

			r1 = math::random< float >( 0.f, 1.f );
			r2 = math::random< float >( 0.f, 6.283185f );
			r3 = math::random< float >( 0.f, 1.f );
			r4 = math::random< float >( 0.f, 6.283185f );

			c1 = std::cos( r2 );
			c2 = std::cos( r4 );
			s1 = std::sin( r2 );
			s2 = std::sin( r4 );

			return { ( c1 * ( r1 * inaccuracy ) ) + ( c2 * ( r3 * spread ) ), ( s1 * ( r1 * inaccuracy ) ) + ( s2 * ( r3 * spread ) ), 0.f };
		}

		bool can_shoot_primary( );
		bool can_shoot_secondary( );
	};
} // namespace sdk
