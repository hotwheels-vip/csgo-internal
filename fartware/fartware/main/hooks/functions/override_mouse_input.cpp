#include "../../features/movement/movement.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::override_mouse_input( void* thisptr, int edx, float* x, float* y )
{
	static auto original = hooks.override_mouse_input.get_original< decltype( &n_detoured_functions::override_mouse_input ) >( );

	if ( !interfaces.m_engine->is_in_game( ) )
		return original( thisptr, edx, x, y );

	if ( movement.m_edgebug_data.m_will_edgebug && *x != 0 ) {
		const float factor =
			100.f /
			( static_cast< float >( std::pow< int, float >(
				  movement.m_edgebug_data.m_ticks_to_stop,
				  ( movement.m_edgebug_data.m_last_tick + movement.m_edgebug_data.m_ticks_to_stop - memory.m_globals->m_tick_count ) / *x ) ) +
		      100.f + ( GET_CONFIG_FLOAT( variables.m_movement.m_edge_bug_lock_amt ) * movement.m_edgebug_data.m_saved_mousedx ) );

		if ( !std::isnan( factor ) )
			*x *= factor;

		/* if ( *y != 0 )
			*y *= 1.f - ( 0.1f * ( variables.m_movement.m_edge_bug_lock_amt * 10.f ) );*/
	}

	original( thisptr, edx, x, y );
}
