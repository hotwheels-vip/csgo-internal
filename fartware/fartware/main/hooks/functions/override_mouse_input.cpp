#include "../hooks.h"
#include "../../features/movement/movement.h"

void __fastcall n_detoured_functions::override_mouse_input(void* thisptr, int edx, float* x, float* y) {
	static auto original = hooks.override_mouse_input.get_original< decltype( &n_detoured_functions::override_mouse_input ) >( );

	if ( !interfaces.m_engine->is_in_game( ) )
		return original( thisptr, edx, x, y );

	original( thisptr, edx, x, y );
}