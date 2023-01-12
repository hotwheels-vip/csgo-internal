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

enum pixelsurf_type_t : int {
	ps_standing = 0,
	ps_ducking
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
		pixelsurf_type_t m_pixelsurf_method{ };

		bool m_will_pixelsurf{ };

		bool m_in_pixelsurf{ };

		int m_ticks_to_stop{ };
		int m_last_tick{ };

		void reset( );
	} m_pixelsurf_data;

	void on_create_move_pre( );
	void on_create_move_post( );

	void handle_edgebug_view_point( );

	void handle_move_data( );

private:
	void detect_edgebug( c_user_cmd* cmd );
};

inline movement_t movement = { };
