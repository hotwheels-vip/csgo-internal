#pragma once
#include "../source_engine/structs/key_bind_t.h"
#include "config.h"

enum e_player_flags {
	player_flag_money = 0,
	player_flag_armor,
	player_flags_max
};

enum e_keybind_indicators {
	key_eb = 0,
	key_ps,
	key_ej,
	key_lj,
	key_dh,
	key_mj,
	key_jb,
	key_max
};

enum e_chams_layers {
	cham_layer_one = 0,
	cham_layer_two,
	cham_layer_three,
	cham_layer_four,
	cham_layer_max
};

class c_chams_settings
{
public:
	bool m_enable_visible   = false;
	c_color m_visible_color = { };

	bool m_enable_invisible   = false;
	c_color m_invisible_color = { };

	bool m_render_original_model = false;

	/* TODO ~ do overlay shit here too */
};

struct variables_t {
	struct visuals_t {
		/* TODO ~ sort this into struct esp_t, and chams_t */

		/* esp settings */
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
		ADD_VARIABLE( float, m_out_of_fov_arrows_size, 20.f );
		ADD_VARIABLE( int, m_out_of_fov_arrows_distance, 200 );

		/* chams settings */
		ADD_VARIABLE( c_chams_settings, m_chams_layer_one,
		              c_chams_settings( false, c_color( 255, 255, 255, 255 ), false, c_color( 255, 255, 255, 255 ), false ) );
		ADD_VARIABLE( c_chams_settings, m_chams_layer_two,
		              c_chams_settings( false, c_color( 255, 255, 255, 255 ), false, c_color( 255, 255, 255, 255 ), false ) );
		ADD_VARIABLE( c_chams_settings, m_chams_layer_three,
		              c_chams_settings( false, c_color( 255, 255, 255, 255 ), false, c_color( 255, 255, 255, 255 ), false ) );
		ADD_VARIABLE( c_chams_settings, m_chams_layer_four,
		              c_chams_settings( false, c_color( 255, 255, 255, 255 ), false, c_color( 255, 255, 255, 255 ), false ) );

		ADD_VARIABLE( bool, m_spectator_list, false );

	} m_visuals;

	struct misc_t {
		ADD_VARIABLE( bool, m_practice_window, false );
		ADD_VARIABLE( key_bind_t, m_practice_cp_key, key_bind_t( 0, 1 ) );
		ADD_VARIABLE( key_bind_t, m_practice_tp_key, key_bind_t( 0, 1 ) );
	} m_misc;

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

		ADD_VARIABLE( bool, m_delay_hop, false );
		ADD_VARIABLE( key_bind_t, m_delay_hop_key, key_bind_t( 0, 1 ) );

		ADD_VARIABLE( bool, m_autostrafe, false );
		ADD_VARIABLE( key_bind_t, m_autostrafe_key, key_bind_t( 0, 1 ) );

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

			ADD_VARIABLE( c_color, m_keybind_color, c_color( 255, 255, 255, 255 ) );
			ADD_VARIABLE( c_color, m_keybind_color_success, c_color( 0, 255, 0, 255 ) );

			ADD_VARIABLE( bool, m_velocity_indicator_fade_alpha, false );
			ADD_VARIABLE( bool, m_velocity_indicator_custom_color, false );
			ADD_VARIABLE( int, m_velocity_indicator_position, 30 );

			ADD_VARIABLE( bool, m_stamina_indicator, false );
			ADD_VARIABLE( bool, m_stamina_indicator_show_pre_speed, false );
			ADD_VARIABLE( c_color, m_stamina_indicator_color1, c_color( 255, 255, 255, 255 ) );
			ADD_VARIABLE( c_color, m_stamina_indicator_color2, c_color( 255, 255, 255, 255 ) );

			ADD_VARIABLE( bool, m_stamina_indicator_fade_alpha, false );
			ADD_VARIABLE( int, m_stamina_indicator_position, 30 );

			ADD_VARIABLE( bool, m_sub_indicators_enable, false );
			ADD_VARIABLE( int, m_sub_indicators_position, 30 );
			ADD_VARIABLE_VECTOR( bool, e_keybind_indicators::key_max, m_sub_indicators, false );

		} m_indicators;
	} m_movement;
};

inline variables_t variables = { };
