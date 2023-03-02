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

namespace n_players
{
	struct impl_t {
		void on_paint_traverse( );

		struct {
			c_base_entity* m_active_weapon = nullptr;
			c_weapon_data* m_weapon_data   = nullptr;
			float m_animated_health        = 1.f;
			float m_ammo                   = 1.f;
		} m_backup_player_data[ 64 ];

		float m_fading_alpha[ 64 ]    = { };
		float m_stored_cur_time[ 64 ] = { };

	private:
		void players( );
	};
} // namespace n_players

inline n_players::impl_t g_players{ };
