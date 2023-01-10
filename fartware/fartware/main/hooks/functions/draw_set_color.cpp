#include "../hooks.h"

void __stdcall n_detoured_functions::draw_set_color( int r, int g, int b, int a )
{
	static auto original = hooks.draw_set_color.get_original< void( __thiscall* )( c_surface*, int, int, int, int ) >( );

	if ( globals.m_console_being_drawn ) {
		original( interfaces.m_surface, r / 2.5, g / 2.3, b / 2.5, a );
		return;
	}

	original( interfaces.m_surface, r, g, b, a );
}
