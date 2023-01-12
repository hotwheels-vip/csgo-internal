#pragma once

struct IDirect3DDevice9;
struct ImDrawList;
struct IDirect3DTexture9;
struct ImFont;

#include "../../../dependencies/steam/isteamclient.h"
#include "../../source_engine/classes/c_color.h"
#include "../../source_engine/classes/c_vector.h"

#include <any>
#include <deque>
#include <functional>
#include <mutex>
#include <shared_mutex>

enum e_text_render_flags {
	text_render_flag_none       = 0,
	text_render_flag_dropshadow = 1,
	text_render_flag_outline    = 2
};

enum e_rect_render_flags {
	rect_render_flag_none          = 0,
	rect_render_flag_inner_outline = ( 1 << 0 ),
	rect_render_flag_outer_outline = ( 1 << 1 )
};

enum e_triangle_render_flags {
	triangle_render_flag_none    = 0,
	triangle_render_flag_outline = ( 1 << 0 ),
	triangle_render_flag_filled  = ( 1 << 1 )
};

enum e_draw_type {
	draw_type_none = 0,
	draw_type_text,
	draw_type_rect,
	draw_type_triangle,
	draw_type_texture
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
	ImU32 m_color                    = { };
	ImU32 m_outline_color            = { };
	e_text_render_flags m_draw_flags = { };
};

struct rect_draw_object_t {
	c_vector_2d m_min                   = { };
	c_vector_2d m_max                   = { };
	ImU32 m_color                       = { };
	ImU32 m_outline_color               = ImColor( 0.f, 0.f, 0.f, 1.f );
	bool m_filled                       = false;
	float m_rounding                    = 0.f;
	ImDrawFlags m_corner_rounding_flags = 0;
	float m_thickness                   = 1.f;
	unsigned int m_outline_flags        = e_rect_render_flags::rect_render_flag_none;
};

struct triangle_draw_object_t {
	c_vector_2d m_first       = { };
	c_vector_2d m_second      = { };
	c_vector_2d m_third       = { };
	ImU32 m_color             = { };
	unsigned int m_draw_flags = e_triangle_render_flags::triangle_render_flag_none;
	ImU32 m_outline_color     = ImColor( 0.f, 0.f, 0.f, 1.f );
	float m_thickness         = 0.f;
};

struct texture_draw_object_t {
	c_vector_2d m_position   = { };
	c_vector_2d m_size       = { };
	ImU32 m_color            = { };
	ImTextureID m_texture_id = { };
	float m_rounding         = { };
	ImDrawFlags m_draw_flags = { };
};

enum e_font_names {
	font_name_verdana_11 = 0,
	font_name_verdana_bd_11,
	font_name_smallest_pixel_10,
	font_name_icon_11,
	font_name_indicator_29,
	font_name_max
};

struct render_t {
	void clear_draw_data( )
	{
		if ( !this->m_draw_data.empty( ) )
			this->m_draw_data.clear( );
	}

	void on_end_scene( const std::function< void( ) >& function, IDirect3DDevice9* device );

	void swap_draw_data( )
	{
		std::unique_lock< std::shared_mutex > lock( this->m_mutex );

		this->m_draw_data.swap( this->m_thread_safe_draw_data );
	}

	void on_release( );

	void text( ImDrawList* draw_list, ImFont* font, const c_vector_2d& position, const std::string& text, const ImU32& color,
	           const ImU32& outline_color     = ImColor( 0.f, 0.f, 0.f, 1.f ),
	           e_text_render_flags draw_flags = e_text_render_flags::text_render_flag_dropshadow );

	void rect( ImDrawList* draw_list, const c_vector_2d& min, const c_vector_2d& max, const ImU32& color,
	           const ImU32& outline_color = ImColor( 0.f, 0.f, 0.f, 1.f ), bool filled = false, float rounding = 0.f,
	           ImDrawFlags corner_rounding_flags = 0, float thickness = 1.f,
	           unsigned int outline_flags = e_rect_render_flags::rect_render_flag_none );

	void copy_and_convert( const uint8_t* rgba_data, uint8_t* out, const size_t size );
	IDirect3DTexture9* steam_image( CSteamID steam_id );

	IDirect3DTexture9* m_counter_terrorist_avatar = nullptr;
	IDirect3DTexture9* m_terrorist_avatar         = nullptr;

	bool world_to_screen( const c_vector& origin, c_vector_2d& screen );

	void render_cached_draw_data( );

	bool m_initialised = false;

	std::deque< draw_object_t > m_draw_data             = { };
	std::deque< draw_object_t > m_thread_safe_draw_data = { };
	std::shared_mutex m_mutex                           = { };

	ImFont* m_fonts[ e_font_names::font_name_max ] = { };
};

inline render_t render = { };
