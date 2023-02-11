#pragma once

#include <any>
#include <deque>
#include <functional>
#include <shared_mutex>
#include <string>

#include "../../game/sdk/classes/c_vector.h"

struct IDirect3DDevice9;
struct ImFont;
struct ImDrawList;
struct ImColor;
class c_vector_2d;

enum e_font_names {
	font_name_verdana_11 = 0,
	font_name_verdana_bd_11,
	font_name_smallest_pixel_10,
	font_name_icon_12,
	font_name_indicator_29,
	font_name_max
};

enum e_draw_type {
	draw_type_none = 0,
	draw_type_text,
	draw_type_max
};

enum e_text_flags {
	text_flag_none       = 0,
	text_flag_dropshadow = 1,
	text_flag_outline    = 2
};

struct draw_object_t {
	draw_object_t( const e_draw_type type, std::any&& obj ) : m_type( type ), m_obj( std::move( obj ) ) { }

	e_draw_type m_type = e_draw_type::draw_type_none;
	std::any m_obj     = { };
};

struct text_draw_object_t {
	ImFont* m_font                   = { };
	c_vector_2d m_position           = { };
	std::string m_text               = { };
	unsigned int m_color             = { };
	unsigned int m_outline_color     = { };
	e_text_flags m_draw_flags        = { };
};

namespace n_render
{
	struct impl_t {
		void clear_draw_data( )
		{
			if ( this->m_draw_data.empty( ) )
				return;

			this->m_draw_data.clear( );
		}

		void on_end_scene( const std::function< void( ) >& function, IDirect3DDevice9* device );

		void swap_draw_data( )
		{
			std::unique_lock< std::shared_mutex > lock( this->m_mutex );

			this->m_draw_data.swap( this->m_thread_safe_draw_data );
		}

		void on_release( );

		void draw_cached_data( );

		/* todo ~ make all these variables private, and make getter functions for them */
		std::deque< draw_object_t > m_draw_data             = { };
		std::deque< draw_object_t > m_thread_safe_draw_data = { };
		std::shared_mutex m_mutex                           = { };

		ImFont* m_fonts[ e_font_names::font_name_max ] = { };

		bool m_initialised = false;

	private:
		void text( ImDrawList* draw_list, ImFont* font, const c_vector_2d& position, const std::string& text, const unsigned int& color,
		           const unsigned int& outline_color, e_text_flags draw_flags = e_text_flags::text_flag_dropshadow );
	};
} // namespace n_render

inline n_render::impl_t g_render{ };