#pragma once

#include "../../memory/memory.h"
#include "../../source_engine/classes/c_prediction.h"
#include "../../source_engine/classes/c_prediction_copy.h"

struct prediction_t {
	void begin( c_user_cmd* cmd );
	void end( ) const;

	void restore_entity_to_predicted_frame( int frame );

	struct prediction_data_t {
		int m_flags         = { };
		int m_movetype      = { };
		c_vector m_velocity = { };
		c_vector m_origin   = { };
	} m_data;

	struct data_map_helper_t {
		data_map_helper_t( const char* pre_action, const char* post_action )
		{
			this->m_pre_action  = pre_action;
			this->m_post_action = post_action;
		}

		void store( unsigned char* start_data );
		void apply( unsigned char* start_data );

		c_prediction_copy m_pred_copy;
		const char* m_pre_action;
		const char* m_post_action;
	};

	c_user_cmd* m_last_cmd  = nullptr;
	c_move_data m_move_data = { };

private:
	unsigned int* m_prediction_random_seed = nullptr;
	c_base_entity** m_prediction_player    = nullptr;

	float m_old_current_time = 0.f;
	float m_old_frame_time   = 0.f;
	int m_old_tick_count     = 0;
	int m_correct_tick_base  = 0;
};

inline prediction_t prediction = { };
