#pragma once

class c_user_cmd;
class c_base_entity;

namespace n_prediction
{
	struct impl_t {
		void begin( c_base_entity* local, c_user_cmd* cmd );
		void end( c_base_entity* local ) const;

		static int get_corrected_tick_base( c_base_entity* local, c_user_cmd* cmd );

	private:
		unsigned int* m_prediction_random_seed = nullptr;
		c_base_entity** m_prediction_player    = nullptr;

		float m_old_current_time{ };
		float m_old_frame_time{ };
		int m_old_tick_count{ };
	};
} // namespace n_prediction

inline n_prediction::impl_t g_prediction{ };