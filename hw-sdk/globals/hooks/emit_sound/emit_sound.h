#pragma once

#include <iostream>

#include "../../../game/sdk/classes/c_base_entity.h"
#include "../../../game/sdk/classes/c_usercmd.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( emit_sound_hook,
	                    void( __fastcall )( void* ecx, void* edx, void* filter, int entity_index, int channel, const char* sound_entry,
	                                        unsigned int sound_entry_hash, const char* sample, float volume, int seed, int sound_level, int flags,
	                                        int pitch, const math::vec3* origin, const math::vec3* direction, void* utl_vec_origins,
	                                        bool update_positions, float soundtime, int speaker_entity, void* sound_params ) );
	struct emit_sound {
		static void __fastcall emit_sound_detour( void* ecx, void* edx, void* filter, int entity_index, int channel, const char* sound_entry,
		                                          unsigned int sound_entry_hash, const char* sample, float volume, int seed, int sound_level,
		                                          int flags, int pitch, const math::vec3* origin, const math::vec3* direction, void* utl_vec_origins,
		                                          bool update_positions, float soundtime, int speaker_entity, void* sound_params );

		static void init( )
		{
			hooks::emit_sound_hook.create( virtual_func::get( g_interfaces.engine_sound, 5 ), emit_sound_detour, _( "emit_sound_detour" ) );
		}

		static void unload( )
		{
			hooks::emit_sound_hook.disable( );
		}
	};
} // namespace hooks
