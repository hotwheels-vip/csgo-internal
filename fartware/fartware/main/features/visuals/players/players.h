#pragma once

class c_base_entity;
class c_weapon_data;

enum e_padding_direction {
	padding_direction_left = 0,
	padding_direction_top,
	padding_direction_right,
	padding_direction_bottom,
	padding_direction_max
};

struct players_t {
	void on_paint_traverse( );

	void spectator_list( );

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
