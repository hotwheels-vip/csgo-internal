#pragma once
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( calculate_view_hook, void( __fastcall )( std::uintptr_t, std::uintptr_t, math::vec3&, math::vec3&, float&, float&, float& ) );

	struct calculate_view {
		static void __fastcall calculate_view_detour( std::uintptr_t, std::uintptr_t, math::vec3&, math::vec3&, float&, float&, float& );

		static void init( )
		{
			hooks::calculate_view_hook.create( g_client_dll.pattern_scan( _( "55 8B EC 83 EC 14 53 56 57 FF 75 18" ) ).as< void* >( ),
			                                   calculate_view_detour, _( "calculate_view_detour" ) );
		}

		static void unload( )
		{
			hooks::calculate_view_hook.disable( );
		}
	};
} // namespace hooks
