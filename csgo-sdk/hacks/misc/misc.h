#pragma once
#include "../../globals/includes/includes.h"
#include <string>

struct IDirect3DTexture9;

namespace n_misc
{
	struct spectator_data_t {
		std::string m_text          = { };
		IDirect3DTexture9* m_avatar = { };
		c_color m_color             = { };
	};

	struct impl_t {
		struct practice_t {
			c_angle saved_angles    = { };
			c_vector saved_position = { };
		} practice;

		void on_create_move_pre( );

		void on_end_scene( );

		void on_paint_traverse( );

	private:
		void practice_window_think( );
		void disable_post_processing( );
		void remove_panorama_blur( );

		void draw_spectator_list( );
		void draw_spectating_local( );
	};
} // namespace n_misc

inline n_misc::impl_t g_misc{ };
