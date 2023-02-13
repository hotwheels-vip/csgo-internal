#pragma once

class c_user_cmd;
class c_base_entity;

namespace n_globals
{
	struct impl_t {
		c_user_cmd* m_cmd      = nullptr;
		c_base_entity* m_local = nullptr;

		bool m_unloading{ };
		// fps lower then server tickrate
		bool m_low_fps = false;

		float m_width{ }, m_height{ };

		float m_last_tick_yaw = 0.f;
	};

} // namespace n_globals

inline n_globals::impl_t g_ctx;
