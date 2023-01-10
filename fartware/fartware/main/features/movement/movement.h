#pragma once

#include "../../convars/convars.h"
#include "../../includes.h"
#include "../../source_engine/classes/c_prediction.h"
#include "../../source_engine/enumerations/e_flags.h"
#include "../../source_engine/enumerations/e_move_types.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../prediction/prediction.h"

struct movement_t {
	struct edgebug_data_t {
		bool will_edgebug{ };
		bool will_fail{ };
		bool should_crouch{ };
		bool strafing{ };

		float yaw_delta;
		float forward_move, side_move;
		float starting_yaw;

		double saved_mousedx{ };
		int ticks_to_stop{ };
		int last_tick{ };
	} edgebug_data;

	void on_create_move_pre( );
	void on_create_move_post( );

	void edgebug( );

private:
	void detect_edgebug( );
};

inline movement_t movement = { };
