#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../game/sdk/classes/c_cs_player.h"
#include "../../../game/sdk/structs/renderable_info.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( list_leaves_in_box_hook, int( __fastcall )( void*, void*, math::vec3&, math::vec3&, unsigned short*, int ) );

	struct list_leaves_in_box {
		static int __fastcall list_leaves_in_box_detour( void* ecx, void* edx, math::vec3& mins, math::vec3& maxs, unsigned short* list,
		                                                 int list_max );

		static void init( )
		{
			hooks::list_leaves_in_box_hook.create( g_engine_dll.pattern_scan( _( "55 8B EC 83 EC ? 8B 4D ? 8D 55" ) ).as< void* >( ),
			                                       list_leaves_in_box_detour, _( "list_leaves_in_box_detour" ) );
		}

		static void unload( )
		{
			hooks::list_leaves_in_box_hook.disable( );
		}
	};
} // namespace hooks
