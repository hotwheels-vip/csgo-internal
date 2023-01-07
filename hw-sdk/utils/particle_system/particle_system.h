#pragma once

// C&P from dalkrhack. Ty dalkr :)

#include "../../game/sdk/classes/c_cs_player.h"
#include "../../game/sdk/classes/c_effect.h"
#include "../../globals/includes/includes.h"

#include "../../globals/ctx/ctx.h"
#include "../keybinds/keybinds.h"

namespace particle_system
{
	struct impl {
	public:
		void* get_particle_system_index( const char* system_name );
		sdk::c_effect* create_effect( void* system, math::vec3* origin );
		void run( math::vec3 origin, const char* effect_name );
		bool init( );
	};
} // namespace particle_system

inline particle_system::impl g_particle_system;
