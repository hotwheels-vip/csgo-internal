#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../hacks/indicators/indicators.h"

void __fastcall n_detoured_functions::paint_traverse( void* ecx, void* edx, unsigned int panel, bool force_repaint, bool force )
{
	static auto original = g_hooks.m_paint_traverse.get_original< decltype( &n_detoured_functions::paint_traverse ) >( );

	const unsigned int panel_hash = HASH_RT( g_interfaces.m_panel->get_panel_name( panel ) );

	original( ecx, edx, panel, force_repaint, force );

	switch ( panel_hash ) {
	case HASH_BT( "MatSystemTopPanel" ): {
		g_render.clear_draw_data( );

		if ( g_ctx.m_local )
			g_indicators.on_paint_traverse( );
		else
			g_indicators.m_indicator_data.reset( );

		g_render.swap_draw_data( );
		break;
	}
	}
}
