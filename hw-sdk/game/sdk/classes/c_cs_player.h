#pragma once
#include <iostream>

#include "../../../globals/includes/includes.h"

#include "../../../utils/convars/convars.h"

#include "c_base_player.h"

namespace sdk
{
	struct c_cs_player : public c_base_player {
	public:
		template< typename T >
		__forceinline T& get( size_t offset )
		{
			return *reinterpret_cast< T* >( reinterpret_cast< std::uintptr_t >( this ) + offset );
		}

		template< typename T >
		__forceinline void set( size_t offset, const T& val )
		{
			*reinterpret_cast< T* >( reinterpret_cast< std::uintptr_t >( this ) + offset ) = val;
		}

		NETVAR( has_defuser, bool, "CCSPlayer", "m_bHasDefuser" );
		NETVAR( gun_game_immunity, bool, "CCSPlayer", "m_bGunGameImmunity" );
		NETVAR( shots_fired, std::int32_t, "CCSPlayer", "m_iShotsFired" );
		NETVAR( eye_angles, math::vec3, "CCSPlayer", "m_angEyeAngles" ); // not sure if [0] is needed.
		NETVAR( armor_value, int, "CCSPlayer", "m_ArmorValue" );
		NETVAR( has_heavy_armor, bool, "CCSPlayer", "m_bHasHeavyArmor" );
		NETVAR( has_helmet, bool, "CCSPlayer", "m_bHasHelmet" );
		NETVAR( is_scoped, bool, "CCSPlayer", "m_bIsScoped" );
		NETVAR( lower_body_yaw_target, float, "CCSPlayer", "m_flLowerBodyYawTarget" );
		NETVAR( flash_max_alpha, float, "CCSPlayer", "m_flFlashMaxAlpha" );
		NETVAR( account, std::int32_t, "CCSPlayer", "m_iAccount" );
		NETVAR( flash_duration, float, "CCSPlayer", "m_flFlashDuration" );
		NETVAR( stamina, float, "CCSPlayer", "m_flStamina" );

		const bool is_enemy( sdk::c_cs_player* player );

		bool is_alive( );

		std::string name( );

		void update_client_side_animation( );

		void set_local_view_angles( const math::vec3& view_angles );
	};
} // namespace sdk
