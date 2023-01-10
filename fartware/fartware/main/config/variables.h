#pragma once
#include "../source_engine/structs/key_bind_t.h"
#include "config.h"

enum e_player_flags {
	player_flag_money = 0,
	player_flag_armor,
	player_flags_max
};

struct variables_t {
	struct visuals_t {
		ADD_VARIABLE( bool, m_bounding_box, false );
		ADD_VARIABLE( c_color, m_bounding_box_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( bool, m_bounding_box_outline, false );

		ADD_VARIABLE( bool, m_player_name, false );
		ADD_VARIABLE( c_color, m_player_name_color, c_color( 255, 255, 255, 255 ) );

		ADD_VARIABLE( bool, m_player_health_bar, false );
		ADD_VARIABLE( bool, m_custom_color_health_bar, false );
		ADD_VARIABLE( c_color, m_player_health_bar_color, c_color( 255, 255, 255, 255 ) );

		ADD_VARIABLE_VECTOR( bool, e_player_flags::player_flags_max, m_player_flags, false );

		ADD_VARIABLE( bool, m_active_weapon_name, false );
		ADD_VARIABLE( c_color, m_active_weapon_name_color, c_color( 255, 255, 255, 255 ) );

		ADD_VARIABLE( bool, m_active_weapon_icon, false );
		ADD_VARIABLE( c_color, m_active_weapon_icon_color, c_color( 255, 255, 255, 255 ) );

		ADD_VARIABLE( bool, m_weapon_ammo_bar, false );
		ADD_VARIABLE( c_color, m_weapon_ammo_bar_color, c_color( 255, 255, 255, 255 ) );

		ADD_VARIABLE( bool, m_player_avatar, false );
		/*ADD_VARIABLE( c_color, m_player_avatar_color, c_color( 255, 255, 255, 255 ) );*/

		ADD_VARIABLE( bool, m_out_of_fov_arrows, false );
		ADD_VARIABLE( c_color, m_out_of_fov_arrows_color, c_color( 255, 255, 255, 255 ) );
	} m_visuals;

	struct movement_t {
		ADD_VARIABLE( bool, m_bunny_hop, false );

		ADD_VARIABLE( bool, m_edge_jump, false );
		ADD_VARIABLE( key_bind_t, m_edge_jump_key, key_bind_t( 0, 1 ) );

		ADD_VARIABLE( bool, m_long_jump, false );
		ADD_VARIABLE( key_bind_t, m_long_jump_key, key_bind_t( 0, 1 ) );

		ADD_VARIABLE( bool, m_mini_jump, false );
		ADD_VARIABLE( key_bind_t, m_mini_jump_key, key_bind_t( 0, 1 ) );

		ADD_VARIABLE( bool, m_jump_bug, false );
		ADD_VARIABLE( key_bind_t, m_jump_bug_key, key_bind_t( 0, 1 ) );

		ADD_VARIABLE( bool, m_edge_bug, false );
		ADD_VARIABLE( bool, m_advanced_detection, false );
		ADD_VARIABLE( key_bind_t, m_edge_bug_key, key_bind_t( 0, 1 ) );
		ADD_VARIABLE( int, m_edge_bug_ticks, 32 );
		ADD_VARIABLE( float, m_edge_bug_lock_amt, 0.3f );

				ADD_VARIABLE( bool, m_pixel_surf, false );
		ADD_VARIABLE( key_bind_t, m_pixel_surf_key, key_bind_t( 0, 1 ) );

		ADD_VARIABLE( bool, m_fast_duck, false );

		ADD_VARIABLE( bool, m_auto_align, false );

		struct indicators_t {
			ADD_VARIABLE( bool, m_velocity_indicator, false );
			ADD_VARIABLE( bool, m_velocity_indicator_show_pre_speed, false );
			/* custom colors */
			ADD_VARIABLE( c_color, m_velocity_indicator_color1, c_color( 255, 255, 255, 255 ) );
			ADD_VARIABLE( c_color, m_velocity_indicator_color2, c_color( 255, 255, 255, 255 ) );
			/* speed based colors */
			ADD_VARIABLE( c_color, m_velocity_indicator_color3, c_color( 255, 199, 89, 255 ) );
			ADD_VARIABLE( c_color, m_velocity_indicator_color4, c_color( 255, 119, 119, 255 ) );
			ADD_VARIABLE( c_color, m_velocity_indicator_color5, c_color( 30, 255, 109, 255 ) );

			ADD_VARIABLE( bool, m_velocity_indicator_fade_alpha, false );
			ADD_VARIABLE( bool, m_velocity_indicator_custom_color, false );
			ADD_VARIABLE( int, m_velocity_indicator_position, 30 );

			ADD_VARIABLE( bool, m_stamina_indicator, false );
			ADD_VARIABLE( bool, m_stamina_indicator_show_pre_speed, false );
			ADD_VARIABLE( c_color, m_stamina_indicator_color1, c_color( 255, 255, 255, 255 ) );
			ADD_VARIABLE( c_color, m_stamina_indicator_color2, c_color( 255, 255, 255, 255 ) );

			ADD_VARIABLE( bool, m_stamina_indicator_fade_alpha, false );
			ADD_VARIABLE( int, m_stamina_indicator_position, 30 );

			ADD_VARIABLE( bool, m_sub_indicators, false );
			ADD_VARIABLE( int, m_sub_indicators_position, 30 );

		} m_indicators;
	} m_movement;
};

inline variables_t variables = { };
