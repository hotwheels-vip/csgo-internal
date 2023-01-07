#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../utils/renderer/renderer.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( is_loose_file_allowed_hook, bool( __fastcall )( void*, void* ) );
	struct is_loose_file_allowed {
		static bool __fastcall is_loose_file_allowed_detour( void* ecx, void* edx );

		static void init( )
		{
			is_loose_file_allowed_hook.create( virtual_func::get( g_interfaces.file_system, 128 ), is_loose_file_allowed_detour,
			                                   _( "is_loose_file_allowed_detour" ) );
		}
		static void unload( )
		{
			is_loose_file_allowed_hook.disable( );
		}
	};
} // namespace hooks
