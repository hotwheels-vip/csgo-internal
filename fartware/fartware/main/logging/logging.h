#pragma once
#include "../memory/memory.h"
#include "../source_engine/classes/c_color.h"
#include "../utilities/render/render.h"
#include <deque>
#include <string>

namespace logging
{
	struct text_t {
		explicit text_t( const std::string& text, const std::string& prefix, float time ) : m_text( text ), m_prefix( prefix ), m_time( time ) { }

		std::string m_text, m_prefix;
		float m_time;
		c_unsigned_char_color m_color = c_unsigned_char_color( 255.f, 255.f, 255.f, 255.f );
	};

	class impl
	{
	public:
		impl( ) : m_text( ){ };

		void print( const std::string& text, const std::string& prefix = "[log]", float time = 8.f )
		{
			m_text.push_back( logging::text_t( text, prefix, time ) );
		};

		void think( )
		{
			// you can change this if u want
			int m_x = 10, m_y = 5, m_size = 15;

			float m_time_left{ };

			// loop through all texts
			for ( std::size_t i = 0; i < m_text.size( ); i++ ) {
				auto& m_curr_noti = m_text[ i ];

				m_curr_noti.m_time -= memory.m_globals->m_frame_time;

				if ( m_curr_noti.m_time <= 0.f // if noti time expired
				     || i > 15 )               // or noti index higher then 15
				{
					// erase
					m_text.erase( m_text.begin( ) + i );
					continue;
				}
			}

			// if text deque empty dont proceed
			if ( m_text.empty( ) )
				return;

			// loop through all texts again
			for ( std::size_t i = 0; i < m_text.size( ); i++ ) {
				auto& m_curr_noti = m_text[ i ];

				// get time so we can manage alpha from it
				m_time_left = m_curr_noti.m_time;

				// if time left smaller then opacity factor
				if ( m_time_left < .5f ) {
					// prevent division by 0
					float f = m_time_left;
					f       = std::clamp( f, 0.f, .5f );

					f /= .5f;

					m_curr_noti.m_color.a = static_cast< int >( f * 255.f );

					// subtract height
					if ( i == 0 && f < 0.2f )
						m_y -= m_size * ( 1.f - f / 0.2f );
				}

				render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						render.m_fonts[ e_font_names::font_name_verdana_bd_11 ], c_vector_2d( m_x, m_y ), m_curr_noti.m_prefix,
						ImColor( Accent[ 0 ], Accent[ 1 ], Accent[ 2 ], m_curr_noti.m_color.a / 255.f ),
						ImColor( 0.f, 0.f, 0.f, m_curr_noti.m_color.a / 255.f ), e_text_render_flags::text_render_flag_dropshadow ) );

				const auto text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, m_curr_noti.m_prefix.c_str( ) );

				render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						render.m_fonts[ e_font_names::font_name_verdana_11 ], c_vector_2d( m_x + text_size.x + 3, m_y ), m_curr_noti.m_text,
						ImColor( m_curr_noti.m_color.r / 255.f, m_curr_noti.m_color.g / 255.f, m_curr_noti.m_color.b / 255.f,
				                 m_curr_noti.m_color.a / 255.f ),
						ImColor( 0.f, 0.f, 0.f, m_curr_noti.m_color.a / 255.f ), e_text_render_flags::text_render_flag_dropshadow ) );

				// apply height to next text
				m_y += m_size;
			}
		}

	private:
		std::deque< logging::text_t > m_text{ };
	};

} // namespace logging

inline logging::impl g_log;
