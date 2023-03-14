#pragma once

class c_user_cmd;
class c_base_entity;
class c_angle;
#include "../../game/sdk/classes/c_vector.h"
#include "../../globals/macros/macros.h"
#include "../../utilities/modules/modules.h"

namespace n_prediction
{
	struct impl_t {
		struct {
			c_vector m_velocity{ };
			c_vector m_origin{ };
			c_angle m_view_angles{ };
			float* m_fall_velocity{ };
			int m_flags{ };
			int m_move_type{ };
		} backup_data;

		void begin( c_base_entity* local, c_user_cmd* cmd );
		void end( c_base_entity* local ) const;
		void update( );
		void restore_entity_to_predicted_frame( int frame );

		static int get_corrected_tick_base( c_base_entity* local, c_user_cmd* cmd );

	private:
		void update_button_state( c_base_entity* local, c_user_cmd* cmd );

		unsigned int* m_prediction_random_seed = nullptr;
		c_base_entity** m_prediction_player    = nullptr;

		float m_old_current_time{ };
		float m_old_frame_time{ };
		int m_old_tick_count{ };
	};
} // namespace n_prediction

inline n_prediction::impl_t g_prediction{ };
