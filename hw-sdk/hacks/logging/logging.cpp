#include "logging.h"
#include "../../game/sdk/structs/color.h"
#include "../../globals/interfaces/interfaces.h"
#include "../../utils/renderer/renderer.h"
#include "../menu/config/config.h"

void logging::impl::print( const std::string text, const std::string prefix, float time )
{
	// push to onscreen logs
	logs.push_back( logging::log_text_t( text, prefix, time ) );
	// print to console
	g_interfaces.convar->console_color_printf( g_config.find< color >( HASH_BT( "m_menu_color" ) ).set_alpha( 255 ),
	                                           console::format( _( "{} {}" ), prefix, text ).c_str( ) );
}

void logging::impl::think( )
{
	int x = 10, y = 5, size = 15;
	color draw_color = color( 255, 255, 255, 255 );
	float left{ };

	for ( std::size_t i = 0U; i < logs.size( ); i++ ) {
		auto& log = logs[ i ];

		log.m_time -= g_interfaces.globals->frame_time;
		if ( log.m_time <= 0.f || i > 15 ) {
			logs.erase( logs.begin( ) + i );
			continue;
		}
	}

	if ( logs.empty( ) )
		return;

	for ( std::size_t i = 0U; i < logs.size( ); i++ ) {
		auto& log = logs[ i ];
		left      = log.m_time;

		if ( left < 0.5f ) {
			float f = left;
			f       = std::clamp( f, 0.f, .5f );

			f /= .5f;

			draw_color.a = static_cast< int >( f * 255.f );

			if ( i == 0 && f < 0.2f )
				y -= size * ( 1.f - f / 0.2f );
		} else
			draw_color.a = 255;

		g_render.render_text( x, y, font_alignment::AL_DEFAULT, font_flags::FLAG_DROPSHADOW, log.m_prefix.c_str( ),
		                      g_fonts[ HASH_BT( "main_verdana_font" ) ],
		                      g_config.find< color >( HASH_BT( "m_menu_color" ) ).set_alpha( draw_color.a ) );

		g_render.render_text( x + 2 + g_render.render_text_size( log.m_prefix.data( ), g_fonts[ HASH_BT( "main_verdana_font" ) ] ).x, y,
		                      font_alignment::AL_DEFAULT, font_flags::FLAG_DROPSHADOW, log.m_text.c_str( ), g_fonts[ HASH_BT( "main_verdana_font" ) ],
		                      draw_color );
		y += size;
	}
}
