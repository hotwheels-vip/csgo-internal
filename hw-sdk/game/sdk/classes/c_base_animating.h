#pragma once
#include <iostream>

#include "../../../globals/includes/includes.h"

#include "c_base_entity.h"

namespace sdk
{
	struct c_base_animating : public c_base_entity {
	public:
		NETVAR( hitbox_set, std::int32_t, "CBaseAnimating", "m_nHitboxSet" );
		NETVAR( cycle, float, "CBaseAnimating", "m_flCycle" );

		std::array< float, 24 >& pose_parameter( )
		{
			static std::uintptr_t offset = netvar::get_table( _( "CBaseAnimating" ), _( "m_flPoseParameter" ) );
			return *reinterpret_cast< std::array< float, 24 >* >( reinterpret_cast< std::uintptr_t >( this ) + offset );
		}

		bool& is_jiggle_bones_enabled( )
		{
			return *reinterpret_cast< bool* >( reinterpret_cast< std::uintptr_t >( this ) + 0x2930 );
		}

		void invalidate_bone_cache( )
		{
			static auto invalidate_bone_cache_address =
				g_client_dll.pattern_scan( _( "80 ? ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 ? ? ? ? ? ? ? ? ? 89 ? ? ? ? ? C3" ) ).as< std::uintptr_t >( );

			reinterpret_cast< void( __thiscall* )( void* ) >( invalidate_bone_cache_address )( this );
		}

		math::vec3 hitbox_position( int index, float point_scale = 0.5f );
		math::vec3 hitbox_position( int index, math::matrix_3x4* matrix, float point_scale = 0.5f );
	};
} // namespace sdk
