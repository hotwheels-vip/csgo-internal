#pragma once

#include "../../../game/sdk/classes/c_cs_player.h"
#include "../../../globals/includes/includes.h"

namespace aimbot
{
	struct impl {
	private:
		sdk::c_cs_player* find_closest( );

	public:
		float hitchance( sdk::c_cs_player* player, math::vec3 angles, sdk::hitgroup hit_group );

		void run( );
	};
} // namespace aimbot

inline aimbot::impl g_aimbot;
