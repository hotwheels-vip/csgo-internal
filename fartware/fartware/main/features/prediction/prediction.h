#pragma once

#include "../../memory/memory.h"
#include "../../source_engine/classes/c_prediction.h"

struct prediction_t {
	void begin( );
	void end( ) const;

	static int get_tick_base( );

	struct prediction_data_t {
		int m_flags         = { };
		c_vector m_velocity = { };
	} m_data;

private:
	unsigned int* m_prediction_random_seed = nullptr;
	c_base_entity** m_prediction_player    = nullptr;
	c_move_data m_move_data                = { };

	float m_old_current_time = 0.f;
	float m_old_frame_time   = 0.f;
	int m_old_tick_count     = 0;
};

inline prediction_t prediction = { };