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
	CREATE_HOOK_HELPER( create_move_hook, __declspec( naked ) void( __fastcall )( sdk::i_client_dll*, void*, int, float, bool ) );

	inline bool can_shoot;

	struct create_move {
		static void __fastcall create_move_detour( sdk::i_client_dll* _this, void* edx, int sequence_number, float input_sample_frametime,
		                                           bool active );

		static void init( )
		{
			hooks::create_move_hook.create( virtual_func::get( g_interfaces.client, 22 ), create_move_detour, _( "create_move_detour" ) );
		}

		static void unload( )
		{
			hooks::create_move_hook.disable( );
		}
	};
} // namespace hooks
