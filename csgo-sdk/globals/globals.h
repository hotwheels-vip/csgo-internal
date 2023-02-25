#pragma once

class c_user_cmd;
class c_base_entity;

#include "../hacks/lagcomp/lagcomp.h"

namespace n_globals
{
	struct impl_t {
		c_user_cmd* m_cmd                     = nullptr;
		c_base_entity* m_local                = nullptr;
		n_lagcomp::impl_t::record_t* m_record = nullptr;

		bool m_unloading{ };

		// fps lower then server tickrate
		bool m_low_fps = false;

		bool m_is_glow_being_drawn = false;

		bool m_is_console_being_drawn = false;

		float m_width{ }, m_height{ };

		float m_last_tick_yaw = 0.f;

		int m_max_allocations = 0;

		int m_last_spectators_y = 5;
	};

} // namespace n_globals

inline n_globals::impl_t g_ctx;
