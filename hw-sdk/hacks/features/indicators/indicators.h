#pragma once

namespace indicators
{
	struct impl {
		struct {
			int last_vel        = 0;
			int tick_prev       = 0;
			int take_off        = 0;
			float take_off_time = 0.f;
			bool last_on_ground = false;
		} m_velocity_info;

		struct {
			int offset = 0;
		} m_keybind_info;

		void reset_data( );
		void render( );
		void velocity( );
		void stamina( );
		void keybinds( );
	};
} // namespace indicators

inline indicators::impl g_indicators;
