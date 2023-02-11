#pragma once

namespace n_indicators
{
	struct impl_t {
		void on_paint_traverse( );

		struct indicator_data_t {
			void reset( )
			{
				this->m_last_velocity           = 0;
				this->m_tick_prev_velocity      = 0;
				this->m_take_off_velocity       = 0;
				this->m_take_off_time_velocity  = 0.f;
				this->m_last_on_ground_velocity = false;
			}

			int m_last_velocity            = 0;
			int m_tick_prev_velocity       = 0;
			int m_take_off_velocity        = 0;
			float m_take_off_time_velocity = 0.f;
			bool m_last_on_ground_velocity = false;
		} m_indicator_data;

	private:
		void velocity( const bool on_ground );
	};
} // namespace n_indicators

inline n_indicators::impl_t g_indicators{ };