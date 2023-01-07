#pragma once
#include "../../../game/sdk/structs/c_anim_state.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( modify_eye_position_hook, void( __fastcall )( sdk::c_anim_state*, void*, math::vec3& ) );

	struct modify_eye_position {
		static void __fastcall modify_eye_position_detour( sdk::c_anim_state*, void*, math::vec3& );

		static void init( )
		{
			hooks::modify_eye_position_hook.create( g_client_dll.pattern_scan( _( "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14" ) ).as< void* >( ),
			                                        modify_eye_position_detour, _( "modify_eye_position_detour" ) );
		}

		static void unload( )
		{
			hooks::modify_eye_position_hook.disable( );
		}
	};
} // namespace hooks
