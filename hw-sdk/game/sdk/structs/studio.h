#pragma once
#include "../../../globals/macros/macros.h"
#include "../../../utils/math/math.h"
#include <minwindef.h>

namespace sdk
{
	struct studio_bbox {
		int bone;
		int group;
		math::vec3 mins;
		math::vec3 maxs;
		int hitbox_name_index;
	};

	struct studio_hitbox_set {
		int hitbox_name_index;
		int hitboxes;
		int hitbox_index;

		char* name( )
		{
			return reinterpret_cast< char* >( reinterpret_cast< std::uintptr_t >( this ) + hitbox_name_index );
		}

		studio_bbox* hitbox( int index )
		{
			return reinterpret_cast< studio_bbox* >( reinterpret_cast< std::uintptr_t >( this ) + hitbox_index ) + index;
		};
	};

	struct studio_bone {
		int name_index;
		int parent;
		PAD( 0x98 );
		int flags;
		PAD( 0x34 );

		char* name( )
		{
			return reinterpret_cast< char* >( reinterpret_cast< std::uintptr_t >( this ) + name_index );
		}
	};

	struct studio_hdr {
		int id;
		int version;
		int checksum;
		char name[ 64 ];
		int length;
		math::vec3 eye_position;
		math::vec3 illum_position;
		math::vec3 hull_min;
		math::vec3 hull_max;
		math::vec3 view_bbmins;
		math::vec3 view_bbmaxs;
		int flags;
		int bones;
		int bone_index;

		int bone_controllers;
		int bone_controller_index;
		int hitbox_sets;
		int hitbox_set_index;

		studio_bone* get_bone( int index )
		{
			return reinterpret_cast< studio_bone* >( reinterpret_cast< std::uintptr_t >( this ) + bone_index ) + index;
		};

		studio_hitbox_set* get_hitbox_set( int index )
		{
			return reinterpret_cast< studio_hitbox_set* >( reinterpret_cast< std::uintptr_t >( this ) + hitbox_set_index ) + index;
		}

		studio_bbox* get_hitbox( int index, int hitbox_set )
		{
			if ( auto set = get_hitbox_set( hitbox_set ) ) {
				return set->hitbox( index );
			}

			return nullptr;
		}

		int get_hitbox_count( int hitbox_set )
		{
			if ( auto set = get_hitbox_set( hitbox_set ) ) {
				return set->hitboxes;
			}

			return 0;
		}
	};
} // namespace sdk
