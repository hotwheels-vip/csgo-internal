#pragma once

class c_view_setup;
struct rect_t;

namespace n_screen
{
	struct impl_t {
		bool on_draw_view_models( c_view_setup& setup );

	private:
		void update_screen_effect_texture( int texture_index, int x, int y, int w, int h, bool dest_fullscreen = false, rect_t* actual_rect = { } );
	};
} // namespace n_screen

inline n_screen::impl_t g_screen{ };