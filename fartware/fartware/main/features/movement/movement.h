#pragma once

#include "../../convars/convars.h"
#include "../../includes.h"
#include "../../source_engine/classes/c_prediction.h"
#include "../../source_engine/enumerations/e_flags.h"
#include "../../source_engine/enumerations/e_move_types.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../prediction/prediction.h"

enum edgebug_type_t : int {
	eb_standing = 0,
	eb_ducking,
	eb_strafing,
	eb_strafing_ducking
};

struct movement_t {
	struct edgebug_data_t {
		edgebug_type_t m_edgebug_method{ };

		bool m_will_edgebug{ };
		bool m_will_fail{ };

		float m_yaw_delta{ };
		float m_starting_yaw{ };

		float m_saved_mousedx{ };
		int m_ticks_to_stop{ };
		int m_last_tick{ };

		void reset( );
	} m_edgebug_data;

	struct pixelsurf_data_t {
		bool m_predicted_succesful = false, m_in_pixel_surf = false, m_should_duck = false;
		int m_prediction_ticks      = 0;
		c_user_cmd* m_simulated_cmd = { };

		void reset( );
	} m_pixelsurf_data;

	void on_create_move_pre( );

	void on_create_move_post( );

	void handle_edgebug_view_point( );

	void handle_move_data( );

	void rotate_movement( c_angle& angle );

private:

	const float direction_yaw( );

	void detect_edgebug( c_user_cmd* cmd );

	void strafe_to_yaw( c_angle& angle, const float yaw );

	float get_perfect_delta( );
};

inline movement_t movement = { };
