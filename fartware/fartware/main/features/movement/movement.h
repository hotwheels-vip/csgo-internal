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
		bool m_will_edgebug{ };
		bool m_will_fail{ };
		bool m_should_crouch{ };
		bool m_should_strafe{ };

		float m_yaw_delta;
		float m_forward_move, m_side_move;
		float m_starting_yaw;

		double m_saved_mousedx{ };
		int m_ticks_to_stop{ };
		int m_last_tick{ };
	} m_edgebug_data;

	struct pixel_surf_data_t {
		bool m_predicted_succesful = false, m_in_pixel_surf = false, m_should_duck = false;
		int m_prediction_ticks      = 0;
		c_user_cmd* m_simulated_cmd = { };
	} m_pixel_surf_data;

	void on_create_move_pre( );
	void on_create_move_post( );

	void edgebug( );

	void handle_edgebug_view_point( );

private:
	void detect_edgebug( );
};

inline movement_t movement = { };
