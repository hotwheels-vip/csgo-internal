#pragma once
#include "../../game/sdk/classes/c_color.h"
#include "config.h"

// TODO ADD THIS
enum e_player_flags {
	player_flag_money = 0,
	player_flag_armor,
	player_flag_reloading,
	player_flag_bomb,
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

namespace n_variables
{
	struct impl_t {
		/* menu */
		ADD_VARIABLE( c_color, m_accent, c_color( 129, 99, 251, 255 ) );

		ADD_VARIABLE( bool, m_aimbot_enable, false );
		ADD_VARIABLE( bool, m_backtrack_enable, false );

		/* visuals - players */
		ADD_VARIABLE( bool, m_players, false );
		ADD_VARIABLE( bool, m_players_box, false );
		ADD_VARIABLE( bool, m_players_box_corner, false );
		ADD_VARIABLE( bool, m_players_box_outline, false );
		ADD_VARIABLE( c_color, m_players_box_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( c_color, m_players_box_outline_color, c_color( 0, 0, 0, 255 ) );
		ADD_VARIABLE( bool, m_players_name, false );
		ADD_VARIABLE( c_color, m_players_name_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( bool, m_players_skeleton, false );
		ADD_VARIABLE( c_color, m_players_skeleton_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( bool, m_glow_enable, false );
		ADD_VARIABLE( c_color, m_glow_vis_color, c_color( 224, 175, 86, 153 ) );
		ADD_VARIABLE( c_color, m_glow_invis_color, c_color( 114, 155, 221, 153 ) );

		/* visuals - edicts */
		ADD_VARIABLE( bool, m_dropped_weapons, false );
		ADD_VARIABLE( bool, m_dropped_weapons_box, false );
		ADD_VARIABLE( bool, m_dropped_weapons_box_corner, false );
		ADD_VARIABLE( bool, m_dropped_weapons_box_outline, false );
		ADD_VARIABLE( c_color, m_dropped_weapons_box_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( c_color, m_dropped_weapons_box_outline_color, c_color( 0, 0, 0, 255 ) );
		ADD_VARIABLE( bool, m_dropped_weapons_name, false );
		ADD_VARIABLE( c_color, m_dropped_weapons_name_color, c_color( 255, 255, 255, 255 ) );

		/* visuals - world */
		ADD_VARIABLE( bool, m_precipitation, false );
		ADD_VARIABLE( int, m_precipitation_type, 0 );
		ADD_VARIABLE( c_color, m_precipitation_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( bool, m_fog, false );
		ADD_VARIABLE( float, m_fog_start, 0.f );
		ADD_VARIABLE( float, m_fog_end, 0.f );
		ADD_VARIABLE( c_color, m_fog_color, c_color( 255, 255, 255, 255 ) );

		ADD_VARIABLE( bool, m_custom_smoke, false );
		ADD_VARIABLE( c_color, m_custom_smoke_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( bool, m_custom_molotov, false );
		ADD_VARIABLE( c_color, m_custom_molotov_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( bool, m_custom_blood, false );
		ADD_VARIABLE( c_color, m_custom_blood_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( bool, m_disable_post_processing, false );
		ADD_VARIABLE( bool, m_remove_panorama_blur, false );

		/* misc - movement */
		ADD_VARIABLE( bool, m_bunny_hop, false );
		ADD_VARIABLE( bool, m_no_crouch_cooldown, false );
		ADD_VARIABLE( bool, m_edge_jump, false );
		ADD_VARIABLE( key_bind_t, m_edge_jump_key, key_bind_t( 0, 1 ) );
		ADD_VARIABLE( bool, m_edge_jump_ladder, false );
		ADD_VARIABLE( bool, m_long_jump, false );
		ADD_VARIABLE( key_bind_t, m_long_jump_key, key_bind_t( 0, 1 ) );
		ADD_VARIABLE( bool, m_mini_jump, false );
		ADD_VARIABLE( key_bind_t, m_mini_jump_key, key_bind_t( 0, 1 ) );
		ADD_VARIABLE( bool, m_jump_bug, false );
		ADD_VARIABLE( key_bind_t, m_jump_bug_key, key_bind_t( 0, 1 ) );
		ADD_VARIABLE( bool, m_pixel_surf, false );
		ADD_VARIABLE( key_bind_t, m_pixel_surf_key, key_bind_t( 0, 1 ) );
		ADD_VARIABLE( bool, m_auto_align, false );
		ADD_VARIABLE( bool, m_auto_duck, false );

		ADD_VARIABLE( bool, m_edge_bug, false );
		ADD_VARIABLE( bool, m_advanced_detection, false );
		ADD_VARIABLE( key_bind_t, m_edge_bug_key, key_bind_t( 0, 1 ) );
		ADD_VARIABLE( int, m_edge_bug_ticks, 32 );
		ADD_VARIABLE( float, m_edge_bug_lock_amt, 0.3f );

		ADD_VARIABLE( bool, m_velocity_indicator, false );
		ADD_VARIABLE( bool, m_velocity_indicator_show_pre_speed, false );
		ADD_VARIABLE( bool, m_velocity_indicator_fade_alpha, false );
		ADD_VARIABLE( bool, m_velocity_indicator_custom_color, false );
		ADD_VARIABLE( int, m_velocity_indicator_padding, 125 );

		/* misc - movement - indicators */
		ADD_VARIABLE( c_color, m_velocity_indicator_color1, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( c_color, m_velocity_indicator_color2, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( c_color, m_velocity_indicator_color3, c_color( 255, 199, 89, 255 ) );
		ADD_VARIABLE( c_color, m_velocity_indicator_color4, c_color( 255, 119, 119, 255 ) );
		ADD_VARIABLE( c_color, m_velocity_indicator_color5, c_color( 30, 255, 109, 255 ) );

		ADD_VARIABLE( bool, m_stamina_indicator, false );
		ADD_VARIABLE( bool, m_stamina_indicator_show_pre_speed, false );
		ADD_VARIABLE( c_color, m_stamina_indicator_color1, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( c_color, m_stamina_indicator_color2, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( bool, m_stamina_indicator_fade_alpha, false );
		ADD_VARIABLE( int, m_stamina_indicator_padding, 125 );

		ADD_VARIABLE( c_color, m_key_color, c_color( 255, 255, 255, 255 ) );
		ADD_VARIABLE( c_color, m_key_color_success, c_color( 0, 255, 0, 255 ) );
		ADD_VARIABLE( bool, m_key_indicators_enable, false );
		ADD_VARIABLE( int, m_key_indicators_position, 100 );
		ADD_VARIABLE_VECTOR( bool, e_keybind_indicators::key_max, m_key_indicators, false );
	};
}; // namespace n_variables

inline n_variables::impl_t g_variables{ };
