#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../game/sdk/classes/c_base_entity.h"
#include "../../../game/sdk/classes/c_cs_player.h"
#include "../../../game/sdk/classes/c_usercmd.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( glow_effect_spectator_hook, bool( __cdecl )( sdk::c_cs_player*, sdk::c_cs_player*, sdk::glow_style&, math::vec3& glow_color,
	                                                                 float&, float&, float&, float&, bool& ) );
	struct glow_effect_spectator {
		static bool __cdecl glow_effect_spectator_detour( sdk::c_cs_player*, sdk::c_cs_player*, sdk::glow_style&, math::vec3&, float&, float&, float&,
		                                                  float&, bool& );

		static void init( )
		{
			hooks::glow_effect_spectator_hook.create( g_client_dll.pattern_scan( _( "55 8B EC 83 EC 14 53 8B 5D 0C 56 57 85 DB 74" ) ).as< void* >( ),
			                                          glow_effect_spectator_detour, _( "glow_effect_spectator_detour" ) );
		}

		static void unload( )
		{
			hooks::glow_effect_spectator_hook.disable( );
		}
	};
} // namespace hooks
