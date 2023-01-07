#pragma once

#include <d3dx9.h>
#include <iostream>

// dependencies
#include "../dependencies/minhook/include/MinHook.h"
#include "../dependencies/mocking_bird/mocking_bird.h"

// game
#include "../game/sdk/classes/c_cs_player.h"
#include "../game/sdk/enums/frame_stage.h"
#include "../game/sdk/enums/net_channel_type.h"
#include "../game/sdk/include.h"
#include "../game/sdk/structs/c_anim_state.h"
#include "../game/sdk/structs/i_net_msg.h"
#include "../game/sdk/structs/renderable_info.h"

// utils
#include "../utils/console/console.h"
#include "../utils/detour/detour.h"
#include "../utils/entity_list/entity_list.h"
#include "../utils/keybinds/keybinds.h"
#include "../utils/vfunc/vfunc.h"

// globals
#include "../globals/ctx/ctx.h"
#include "../globals/interfaces/interfaces.h"

// hacks
#include "../hacks/features/aimbot/aimbot.h"
#include "../hacks/features/indicators/indicators.h"
#include "../hacks/features/lagcomp/lagcomp.h"
#include "../hacks/features/movement/movement.h"
#include "../hacks/features/visuals/visuals.h"
#include "../hacks/logging/logging.h"
#include "../hacks/prediction/prediction.h"

// imgui
#include "../hacks/menu/imgui/imgui_helper.h"

namespace hooks
{
	struct impl {
		bool init( );
		void unload( );
	};
} // namespace hooks

inline hooks::impl g_hooks;

#include "hooks.inl"
