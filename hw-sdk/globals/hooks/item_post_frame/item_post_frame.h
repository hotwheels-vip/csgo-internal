#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../game/sdk/classes/c_base_entity.h"
#include "../../../game/sdk/classes/c_usercmd.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( item_post_frame_hook, void( __fastcall )( void*, void* ) );

	struct item_post_frame {
		static void __fastcall item_post_frame_detour( void* self, void* edx );

		static void init( )
		{
			hooks::item_post_frame_hook.create( g_client_dll.pattern_scan( _( "55 8B EC 83 EC ? 56 57 8B F9 8B 37" ) ).as< void* >( ),
			                                    item_post_frame_detour, _( "item_post_frame_detour" ) );
		}

		static void unload( )
		{
			hooks::item_post_frame_hook.disable( );
		}
	};
} // namespace hooks
