#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../game/sdk/structs/i_net_msg.h"
#include "../../../utils/renderer/renderer.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( send_net_msg_hook, bool( __fastcall )( void*, void*, sdk::i_net_msg*, bool, bool ) );
	struct send_net_msg {
		static bool __fastcall send_net_msg_detour( void* ecx, void* edx, sdk::i_net_msg* net_message, bool reliable, bool voice );

		static void init( )
		{
			send_net_msg_hook.create( g_engine_dll.pattern_scan( _( "55 8B EC 83 EC 08 56 8B F1 8B 4D 04 E8s" ) ).as< void* >( ), send_net_msg_detour,
			                          _( "send_net_msg_detour" ) );
		}
		static void unload( )
		{
			send_net_msg_hook.disable( );
		}
	};
} // namespace hooks
