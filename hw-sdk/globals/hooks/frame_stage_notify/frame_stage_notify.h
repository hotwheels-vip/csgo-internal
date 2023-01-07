#pragma once
#include "../hooking.h"
#include <iostream>

#include "../../../game/sdk/classes/c_base_entity.h"
#include "../../../game/sdk/classes/c_usercmd.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../ctx/ctx.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

#include "../../../game/sdk/enums/frame_stage.h"
#include "../../../hacks/features/lagcomp/lagcomp.h"
#include "../../../hacks/features/visuals/visuals.h"
#include "../../../utils/entity_list/entity_list.h"

namespace hooks
{
	CREATE_HOOK_HELPER( frame_stage_notify_hook, void( __fastcall )( void*, void*, sdk::frame_stage ) );

	struct frame_stage_notify {
		static void __fastcall frame_stage_notify_detour( void* ecx, void* edx, sdk::frame_stage stage );

		static void init( )
		{
			hooks::frame_stage_notify_hook.create( virtual_func::get( g_interfaces.client, 37 ), frame_stage_notify_detour,
			                                       _( "frame_stage_notify_detour" ) );
		}

		static void unload( )
		{
			hooks::frame_stage_notify_hook.disable( );
		}
	};
} // namespace hooks
