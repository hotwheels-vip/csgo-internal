#pragma once
#pragma once

#include <any>
#include <deque>
#include <functional>
#include <shared_mutex>
#include <string>

#include "../../dependencies/steam/isteamclient.h"
#include "../../game/sdk/classes/c_vector.h"

struct IDirect3DDevice9;
struct IDirect3DTexture9;
struct ImFont;
struct ImDrawList;
struct ImColor;
class c_vector_2d;
class c_vector;

enum e_font_names {
	font_name_verdana_11 = 0,
	font_name_verdana_bd_11,
	font_name_smallest_pixel_10,
	font_name_icon_12,
	font_name_indicator_29,
	font_name_tahoma_12,
	font_name_tahoma_bd_12,
	font_name_icon_13,
	font_name_max
};

enum e_draw_type {
	draw_type_none = 0,
	draw_type_text,
	draw_type_line,
	draw_type_rect,
	draw_type_triangle,
	draw_type_max
};

enum e_text_flags {
	text_flag_none       = 0,
	text_flag_dropshadow = 1,
	text_flag_outline    = 2
};

enum e_rect_flags {
	rect_flag_none          = 0,
	rect_flag_inner_outline = 1,
	rect_flag_outer_outline = 2
};

enum e_triangle_flags {
	triangle_flag_none    = 0,
	triangle_flag_outline = 1,
	triangle_flag_filled  = 2
};

struct draw_object_t {
	draw_object_t( const e_draw_type type, std::any&& obj ) : m_type( type ), m_obj( std::move( obj ) ) { }

	e_draw_type m_type = e_draw_type::draw_type_none;
	std::any m_obj     = { };
};

struct text_draw_object_t {
	ImFont* m_font               = { };
	c_vector_2d m_position       = { };
	std::string m_text           = { };
	unsigned int m_color         = { };
	unsigned int m_outline_color = { };
	e_text_flags m_draw_flags    = { };
};

struct line_draw_object_t {
	c_vector_2d m_start  = { };
	c_vector_2d m_end    = { };
	unsigned int m_color = 0x0;
	float m_thickness    = 0.f;
};

struct rect_draw_object_t {
	c_vector_2d m_min            = { };
	c_vector_2d m_max            = { };
	unsigned int m_color         = { };
	unsigned int m_outline_color = { };
	bool m_filled                = false;
	float m_rounding             = 0.f;
	int m_corner_rounding_flags  = 0;
	float m_thickness            = 1.f;
	unsigned int m_outline_flags = e_rect_flags::rect_flag_none;
};

struct triangle_draw_object_t {
	c_vector_2d m_first          = { };
	c_vector_2d m_second         = { };
	c_vector_2d m_third          = { };
	unsigned int m_color         = { };
	unsigned int m_draw_flags    = e_triangle_flags::triangle_flag_none;
	unsigned int m_outline_color = { };
	float m_thickness            = 0.f;
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

		bool world_to_screen( const c_vector& origin, c_vector_2d& screen );

		/* todo ~ make all these variables private, and make getter functions for them */
		std::deque< draw_object_t > m_draw_data             = { };
		std::deque< draw_object_t > m_thread_safe_draw_data = { };
		std::shared_mutex m_mutex                           = { };

		ImFont* m_fonts[ e_font_names::font_name_max ] = { };

		bool m_initialised = false;

		void text( ImDrawList* draw_list, ImFont* font, const c_vector_2d& position, const std::string& text, const unsigned int& color,
		           const unsigned int& outline_color, e_text_flags draw_flags = e_text_flags::text_flag_dropshadow );

		void rect( ImDrawList* draw_list, const c_vector_2d& min, const c_vector_2d& max, const unsigned int& color,
		           const unsigned int& outline_color, bool filled, float rounding, int corner_rounding_flags, float thickness,
		           unsigned int outline_flags );

		void corner_rect( float x1, float y1, float x2, float y2, const unsigned int& color, float thickness = 1.f );

		void copy_and_convert( const uint8_t* rgba_data, uint8_t* out, const size_t size );
		IDirect3DTexture9* steam_image( CSteamID steam_id );

		IDirect3DTexture9* m_counter_terrorist_avatar = nullptr;
		IDirect3DTexture9* m_terrorist_avatar         = nullptr;
	};
} // namespace n_render

inline n_render::impl_t g_render{ };
