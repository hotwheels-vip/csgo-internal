#pragma once

#include <functional>

struct IDirect3DDevice9;
struct ImFont;

enum e_font_names {
	font_name_verdana_11 = 0,
	font_name_verdana_bd_11,
	font_name_smallest_pixel_10,
	font_name_icon_12,
	font_name_indicator_29,
	font_name_max
};

namespace n_render
{
	struct impl_t {
		void on_end_scene( const std::function< void( ) >& function, IDirect3DDevice9* device );
		void on_release( );

		/* todo ~ make all these variables private, and make getter functions for them */
		ImFont* m_fonts[ e_font_names::font_name_max ] = { };

		bool m_initialised = false;
	};
} // namespace n_render

inline n_render::impl_t g_render{ };