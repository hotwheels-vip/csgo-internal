#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../utils/renderer/renderer.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( end_scene_hook, LONG( __stdcall )( IDirect3DDevice9* ) );
	struct end_scene {
		static LONG __stdcall end_scene_detour( IDirect3DDevice9* device );

		static void init( )
		{
			g_render.init( g_interfaces.device );

			end_scene_hook.create( virtual_func::get( g_interfaces.device, 42 ), end_scene_detour, _( "end_scene_detour" ) );
		}
		static void unload( )
		{
			end_scene_hook.disable( );
		}
	};
} // namespace hooks
