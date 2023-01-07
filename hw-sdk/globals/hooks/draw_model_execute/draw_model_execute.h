#pragma once

#include <iostream>

#include "../../../game/sdk/classes/c_cs_player.h"
#include "../../../game/sdk/structs/model_render_info.h"
#include "../../../utils/math/types/matrix.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( draw_model_execute_hook, void( __fastcall )( void*, void*, void*, void*, sdk::model_render_info&, math::matrix_3x4* ) );
	struct draw_model_execute {
		static void __fastcall draw_model_execute_detour( void* ecx, void* edx, void* context, void* state, sdk::model_render_info& info,
		                                                  math::matrix_3x4* custom_bone_to_world );

		static void init( )
		{
			hooks::draw_model_execute_hook.create( virtual_func::get( g_interfaces.model_render, 21 ), draw_model_execute_detour,
			                                       _( "draw_model_execute_detour" ) );
		}

		static void unload( )
		{
			hooks::draw_model_execute_hook.disable( );
		}
	};
} // namespace hooks
