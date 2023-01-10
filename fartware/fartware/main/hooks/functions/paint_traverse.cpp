#include "../hooks.h"

#include "../../features/entities/entities.h"
#include "../../features/movement/indicators/indicators.h"
#include "../../features/visuals/players/players.h"

void __fastcall n_detoured_functions::paint_traverse( c_surface* thisptr, int edx, unsigned int panel, bool force_repaint, bool force )
{
	static auto original = hooks.paint_traverse.get_original< decltype( &n_detoured_functions::paint_traverse ) >( );

	const unsigned int panel_hash = fnv1a::hash( interfaces.m_panel->get_panel_name( panel ) );

	static bool print_to_console = []( ) {
		interfaces.m_engine->client_cmd_unrestricted( "clear" );
		interfaces.m_engine->client_cmd_unrestricted( "echo injection succesfull.\n" );

		const char8_t* hook[] = {
			( u8"\n" ),
			( u8"\n" ),
			( u8"⣿⣿⣿⣿⣿⣿⣿⡿⠟⠛⠉⠉⠉⠉⠋⠉⠉⠙⠛⠛⠻⠿⢿⣿⣿⣿⣿⣿⣿⣿\n" ),
			( u8"⣿⣿⣿⣿⠿⠋⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠈⠉⠻⣿⣿⣿⣿⣿\n" ),
			( u8"⣿⣿⡟⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠙⢻⣿⣿⣿\n" ),
			( u8"⣿⠏⠄⠄⠄⠄⠄⠄⠄⠄⢀⣔⢤⣄⡀⠄⡄⡀⠄⠄⠄⠄⠄⠄⠄⠄⠄⢻⣿⣿\n" ),
			( u8"⠏⠄⠄⠄⠄⠄⠄⠄⢀⣀⣨⣵⣿⣿⣿⣿⣧⣦⣤⣀⣿⣷⡐⠄⠄⠄⠄⠄⢿⣿\n" ),
			( u8"⠄⠄⠄⠄⠄⠄⠐⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠄⠄⠄⢚⣿\n" ),
			( u8"⣆⠄⠄⠄⠄⠄⠄⢻⣿⣿⣿⣿⡿⠛⠛⠛⠛⣿⢿⣿⣿⣿⡿⢟⣻⣄⣤⣮⡝⣿\n" ),
			( u8"⣿⠆⠄⠄⠄⠄⠄⠄⠄⠄⠉⠘⣿⡗⡕⣋⢉⣩⣽⣬⣭⣶⣿⣿⣿⣿⣝⣻⣷⣿\n" ),
			( u8"⣿⣦⡀⠄⠄⠠⢀⠄⠄⠁⠄⠄⣿⣿⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣿\n" ),
			( u8"⣿⣿⣿⡆⠄⠄⠰⣶⡗⠄⠄⠄⣿⣿⣿⣿⣦⣌⠙⠿⣿⣿⣿⣿⣿⣿⣿⡛⠱⢿\n" ),
			( u8"⣿⣿⣿⣿⡀⠄⠄⠿⣿⠄⠄⠄⠨⡿⠿⠿⣿⣟⣿⣯⣹⣿⣿⣿⣿⣿⣿⣿⣦⡀\n" ),
			( u8"⣿⣿⣿⣿⣷⠄⠄⠄⢷⣦⠄⠄⠐⢶⢾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇\n" ),
			( u8"⣿⣿⣿⣿⣿⣧⡄⠄⠄⠉⠄⠄⠄⢉⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠄\n" ),
			( u8"⣿⣿⣿⣿⣿⠟⠋⠄⠄⠄⠄⠄⠄⠈⠛⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠄⠄\n" ),
			( u8"⣿⠿⠛⠉⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠘⠿⢿⣿⣿⣿⣿⣿⠿⠋⠄⠄⠄⠄\n" ),
			( u8"\n" ),
		};

		for ( auto& i : hook )
			interfaces.m_convar->console_printf( ( "echo %s" ), i );

		return true;
	}( );

	if ( panel_hash == fnv1a::hash( "GameConsole" ) || panel_hash == fnv1a::hash( "CompletionList" ) ) {
		globals.m_console_being_drawn = true;
		original( thisptr, edx, panel, force_repaint, force );
		globals.m_console_being_drawn = false;
		return;
	}

	original( thisptr, edx, panel, force_repaint, force );

	if ( panel_hash == fnv1a::hash_const( "FocusOverlayPanel" ) ) {
		render.clear_draw_data( );

		players.on_paint_traverse( );
		indicators.on_paint_traverse( );

		render.swap_draw_data( );
	}
}
