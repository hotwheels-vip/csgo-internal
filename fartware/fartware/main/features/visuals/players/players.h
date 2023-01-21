#pragma once

class c_base_entity;
class c_weapon_data;
struct model_render_info_t;
struct matrix3x4_t;

enum e_padding_direction {
	padding_direction_left = 0,
	padding_direction_top,
	padding_direction_right,
	padding_direction_bottom,
	padding_direction_max
};

struct players_t {
	void on_paint_traverse( );
	void on_draw_model_execute( int ecx, int edx, void* context, void* state, model_render_info_t* info, matrix3x4_t* bone_to_world );
	bool on_attach( );

	void on_end_scene( );

private:
	struct backup_player_data_t {
		c_base_entity* m_active_weapon = nullptr;
		c_weapon_data* m_weapon_data   = nullptr;
		float m_health                 = 1.f;
		float m_ammo                   = 1.f;
	} m_backup_player_data[ 64 ];

	float m_fading_alpha[ 64 ] = { };
};

inline players_t players = { };
