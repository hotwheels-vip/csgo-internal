#include "indicators.h"
#include "../../../globals/ctx/ctx.h"
#include "../../menu/config/config.h"

void indicators::impl::reset_data( )
{
	m_velocity_info.last_vel       = 0;
	m_velocity_info.tick_prev      = 0;
	m_velocity_info.take_off       = 0;
	m_velocity_info.take_off_time  = 0.f;
	m_velocity_info.last_on_ground = false;
}

void indicators::impl::render( )
{
	if ( !g_interfaces.engine->is_fully_connected( ) || !g_ctx.local ) {
		reset_data( );
		return;
	}

	if ( g_config.find< bool >( HASH_BT( "m_velocity_indicator" ) ) )
		g_indicators.velocity( );

	if ( g_config.find< bool >( HASH_BT( "m_stamina_indicator" ) ) )
		g_indicators.stamina( );
}

void indicators::impl::velocity( )
{
	int corrected_velocity = static_cast< int >( ROUND_UP( g_ctx.local->velocity( ).length_2d( ) ) );

	if ( m_velocity_info.last_on_ground && !g_ctx.local->flags( ).has( sdk::flags::ONGROUND ) ) {
		m_velocity_info.take_off      = corrected_velocity;
		m_velocity_info.take_off_time = g_interfaces.globals->current_time + 2.f;
	}

	float text_alpha = g_config.find< bool >( HASH_BT( "m_velocity_indicator_fade" ) )
	                       ? g_ctx.local->velocity( ).length_2d( ) >= 310.f ? 310.f : g_ctx.local->velocity( ).length_2d( ) / 310.f
	                       : 1.f;

	color velocity_color = corrected_velocity == m_velocity_info.last_vel  ? color( 255, 199, 89, text_alpha * 255 )
	                       : corrected_velocity < m_velocity_info.last_vel ? color( 255, 119, 119, text_alpha * 255 )
	                                                                       : color( 30, 255, 109, text_alpha * 255 );
	const bool should_draw_pre =
		( !g_ctx.local->flags( ).has( sdk::flags::ONGROUND ) || ( m_velocity_info.take_off_time > g_interfaces.globals->current_time ) );

	const std::string str_pre = std::format( ( "{} ({})" ), corrected_velocity, m_velocity_info.take_off ),
					  str     = std::format( ( "{}" ), corrected_velocity );

	g_render.render_text( g_ctx.screen_size.x / 2, g_ctx.screen_size.y / 2, font_alignment::AL_HORIZONTAL_CENTER, font_flags::FLAG_DROPSHADOW,
	                      g_config.find< bool >( HASH_BT( "m_velocity_indicator_show_pre" ) ) && should_draw_pre ? str_pre.c_str( ) : str.c_str( ),
	                      g_fonts[ HASH_BT( "indicator_verdana_font" ) ], velocity_color );

	if ( m_velocity_info.tick_prev + 5 < g_interfaces.globals->tick_count ) {
		m_velocity_info.last_vel  = corrected_velocity;
		m_velocity_info.tick_prev = g_interfaces.globals->tick_count;
	}

	m_velocity_info.last_on_ground = g_ctx.local->flags( ).has( sdk::flags::ONGROUND );
}

void indicators::impl::stamina( ) { }

void indicators::impl::keybinds( ) { }
