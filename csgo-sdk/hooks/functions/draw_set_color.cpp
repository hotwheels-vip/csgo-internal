#include "../../globals/globals.h"
#include "../../globals/interfaces/interfaces.h"
#include "../hooks.h"

void __stdcall n_detoured_functions::draw_set_color( int r, int g, int b, int a )
{
	static auto original = g_hooks.m_draw_set_color.get_original< void( __thiscall* )( c_surface*, int, int, int, int ) >( );

	if ( g_ctx.m_is_console_being_drawn ) {
		original( g_interfaces.m_surface, r / 2.5, g / 2.3, b / 2.5, a );
		return;
	}

	original( g_interfaces.m_surface, r, g, b, a );
}
