#pragma once
#include "../../../source_engine/classes/c_vector.h"
#include "../../../source_engine/enumerations/e_client_frame_stage.h"
#include "../../../source_engine/structs/matrix_t.h"
#include "../../../source_engine/structs/rect_t.h"

class c_view_setup;

struct motion_blur_data_t {
	float m_values[ 4 ]          = { 0.f, 0.f, 0.f, 0.f };
	float m_viewport_values[ 4 ] = { 0.f, 0.f, 1.f, 1.f };
};

inline motion_blur_data_t motion_blur_data = { };

struct world_t {
	void on_frame_stage_notify( e_client_frame_stage );
	bool on_draw_view_models( c_view_setup& setup );

private:
	void update_screen_effect_texture( int texture_index, int x, int y, int w, int h, bool dest_fullscreen = false, rect_t* actual_rect = NULL );
};

struct motion_blur_history_t {
	motion_blur_history_t( )
	{
		this->m_last_time_update                = 0.0f;
		this->m_previous_pitch                  = 0.0f;
		this->m_previous_yaw                    = 0.0f;
		this->m_previous_position               = c_vector( 0.0f, 0.0f, 0.0f );
		this->m_previous_frame_basis_vectors    = { };
		this->m_no_rotational_motion_blur_until = 0.0f;
	}

	float m_last_time_update;
	float m_previous_pitch;
	float m_previous_yaw;
	c_vector m_previous_position;
	matrix3x4_t m_previous_frame_basis_vectors;
	float m_no_rotational_motion_blur_until;
};

inline world_t world = { };
