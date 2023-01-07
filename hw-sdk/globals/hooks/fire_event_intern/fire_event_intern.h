#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../game/sdk/structs/i_game_event.h"
#include "../../../utils/renderer/renderer.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( fire_event_intern_hook, bool( __fastcall )( void*, void*, sdk::i_game_event*, bool, bool ) );
	struct fire_event_intern {
		static bool __fastcall fire_event_intern_detour( void* ecx, void* edx, sdk::i_game_event* game_event, bool server_only, bool client_only );

		static void init( )
		{
			fire_event_intern_hook.create(
				g_engine_dll.pattern_scan( _( "55 8B EC 83 E4 ? 83 EC ? 53 8B D9 56 57 89 5C 24 ? 8D B3" ) ).as< void* >( ), fire_event_intern_detour,
				_( "fire_event_intern_detour" ) );
		}
		static void unload( )
		{
			fire_event_intern_hook.disable( );
		}
	};
} // namespace hooks
