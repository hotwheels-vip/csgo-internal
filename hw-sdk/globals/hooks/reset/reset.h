#pragma once
#include "../hooking.h"
#include <d3d9.h>

#include "../../ctx/ctx.h"
#include "../../interfaces/interfaces.h"
#include "../../../dependencies/imgui/imgui.h"

namespace hooks
{
	CREATE_HOOK_HELPER( reset_hook, long( __stdcall )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) );

	struct reset {
		static long __stdcall reset_detour( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters );

		static void init( )
		{
			hooks::reset_hook.create( virtual_func::get( g_interfaces.device, 16 ), reset_detour,
			                                       _( "reset_detour" ) );
		}

		static void unload( )
		{
			hooks::reset_hook.disable( );
		}
	};
} // namespace hooks
