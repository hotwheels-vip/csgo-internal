#pragma once

namespace n_indicators
{
	struct impl_t {
		void on_paint_traverse( );

		struct indicator_data_t {
			void reset( )
			{
				this->last_velocity           = 0;
				this->tick_prev_velocity      = 0;
				this->take_off_velocity       = 0;
				this->take_off_time_velocity  = 0.f;
				this->last_on_ground_velocity = false;
			}

			int last_velocity            = 0;
			int tick_prev_velocity       = 0;
			int take_off_velocity        = 0;
			float take_off_time_velocity = 0.f;
			bool last_on_ground_velocity = false;
		} m_indicator_data;
	};
} // namespace n_indicators

inline n_indicators::impl_t g_indicators{ };