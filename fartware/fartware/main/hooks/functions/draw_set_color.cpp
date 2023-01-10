#include "../hooks.h"

void __stdcall n_detoured_functions::draw_set_color( int r, int g, int b, int a )
{
	static auto original = hooks.draw_set_color.get_original< decltype( &n_detoured_functions::draw_set_color ) >( );

	if ( globals.m_console_being_drawn ) {
		original( r / 2.5, g / 2.3, b / 2.5, a / 0.95 );
		return;
	}

	original( r, g, b, a );
}
