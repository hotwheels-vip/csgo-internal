#pragma once
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( alloc_key_value_memory_hook, void*( __fastcall )( void* thisptr, int edx, int size ) );

	struct alloc_key_value_memory {
		static void* __fastcall alloc_key_value_memory_detour( void* thisptr, int edx, int size );

		static void init( )
		{
			hooks::alloc_key_value_memory_hook.create( virtual_func::get( g_interfaces.key_values_system, 1 ), alloc_key_value_memory_detour,
			                                           _( "alloc_key_value_memory_detour" ) );
		}

		static void unload( )
		{
			hooks::alloc_key_value_memory_hook.disable( );
		}
	};
} // namespace hooks
