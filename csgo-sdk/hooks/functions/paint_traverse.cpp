#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::paint_traverse( void* ecx, void* edx, unsigned int panel, bool force_repaint, bool force )
{
	static auto original = g_hooks.m_paint_traverse.get_original< decltype( &n_detoured_functions::paint_traverse ) >( );

	const unsigned int panel_hash = HASH_RT( g_interfaces.m_panel->get_panel_name( panel ) );

	original( ecx, edx, panel, force_repaint, force );

	switch ( panel_hash ) {
	case HASH_BT( "FocusOverlayPanel" ): {
		if ( g_globals.local ) { }
		break;
	}
	}
}
