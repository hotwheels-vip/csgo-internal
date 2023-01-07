#pragma once
#include "../../game/sdk/classes/c_base_combat_weapon.h"
#include "../../game/sdk/classes/c_cs_player.h"
#include "../../game/sdk/classes/c_usercmd.h"
#include "../../game/sdk/enums/buttons.h"
#include "../../game/sdk/enums/move_type.h"
#include "../../hacks/features/lagcomp/lagcomp.h"

namespace ctx
{
	struct impl {
		bool init( );

		sdk::c_cs_player* local           = nullptr;
		sdk::c_user_cmd* cmd              = nullptr;
		sdk::c_base_combat_weapon* weapon = nullptr;
		lagcomp::record* record           = nullptr;

		struct {
			sdk::c_user_cmd* cmd    = nullptr;
			sdk::c_cs_player* local = nullptr;
		} backup;

		math::vec2< int > screen_size = { };
		math::matrix_4x4 view_matrix  = { };

		bool running_post_think = false;
		bool updating_animations = false;
	};
} // namespace ctx

inline ctx::impl g_ctx{ };
