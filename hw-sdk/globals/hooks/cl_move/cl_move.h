#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../utils/renderer/renderer.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	inline auto send_packet = false;
	inline auto shifting_tb = false;

	CREATE_HOOK_HELPER( cl_move_hook, void( __cdecl )( float, bool ) );
	struct cl_move {
		static void __cdecl cl_move_detour( float accumulated_extra_samples, bool final_tick );

		static void init( )
		{
			cl_move_hook.create( g_engine_dll.pattern_scan( _( "55 8B EC 81 EC ? ? ? ? 53 56 8A F9" ) ).as< void* >( ), cl_move_detour,
			                     _( "cl_move_detour" ) );
		}
		static void unload( )
		{
			cl_move_hook.disable( );
		}
	};
} // namespace hooks
