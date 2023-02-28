#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../../globals/logger/logger.h"
#include "../hooks.h"

#include "../../hacks/debug/debug.h"
#include "../../hacks/indicators/indicators.h"
#include "../../hacks/misc/misc.h"
#include "../../hacks/visuals/edicts/edicts.h"
#include "../../hacks/visuals/players/dormancy/dormancy.h"
#include "../../hacks/visuals/players/players.h"

void __fastcall n_detoured_functions::paint_traverse( void* ecx, void* edx, unsigned int panel, bool force_repaint, bool force )
{
	static auto original = g_hooks.m_paint_traverse.get_original< decltype( &n_detoured_functions::paint_traverse ) >( );

	const unsigned int panel_hash = HASH_RT( g_interfaces.m_panel->get_panel_name( panel ) );

	if ( panel_hash == HASH_BT( "GameConsole" ) || panel_hash == HASH_BT( "CompletionList" ) ) {
		g_ctx.m_is_console_being_drawn = true;
		original( ecx, edx, panel, force_repaint, force );
		g_ctx.m_is_console_being_drawn = false;
		return;
	}

	original( ecx, edx, panel, force_repaint, force );

	switch ( panel_hash ) {
	case HASH_BT( "MatSystemTopPanel" ): {
		g_render.clear_draw_data( );

		if ( g_render.m_initialised ) {
			g_ctx.m_low_fps = static_cast< int >( ImGui::GetIO( ).Framerate + 0.5f ) <
			                  static_cast< int >( 1.f / g_interfaces.m_global_vars_base->m_interval_per_tick );

			g_misc.on_paint_traverse( );

			g_logger.on_paint_traverse( );

#ifdef _DEBUG
			g_debugger.on_paint_traverse( );
#endif

			if ( g_ctx.m_local ) {
				g_indicators.on_paint_traverse( );
				g_edicts.on_paint_traverse( );
				g_dormancy.start( );
				g_players.on_paint_traverse( );
				g_dormancy.finish( );
			} else
				g_indicators.m_indicator_data.reset( );
		}

		g_render.swap_draw_data( );
		break;
	}
	}
}
