#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( paint_traverse_hook, void( __fastcall )( sdk::i_panel*, void*, unsigned int, bool, bool ) );

	struct paint_traverse {
		static void __fastcall paint_traverse_detour( sdk::i_panel*, void*, unsigned int panel, bool force_repaint, bool allow_force );

		static void init( )
		{
			hooks::paint_traverse_hook.create( virtual_func::get( g_interfaces.panel, 41 ), paint_traverse_detour, _( "paint_traverse_detour" ) );
		}

		static void unload( )
		{
			hooks::paint_traverse_hook.disable( );
		}
	};
} // namespace hooks
