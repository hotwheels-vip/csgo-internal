#include "../../game/sdk/classes/c_engine_client.h"
#include "../../game/sdk/classes/c_global_vars_base.h"
#include "../../globals/includes/includes.h"
#include "../../hacks/movement/movement.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::override_mouse_input( void* thisptr, int edx, float* x, float* y )
{
	static auto original = g_hooks.m_override_mouse_input.get_original< decltype( &n_detoured_functions::override_mouse_input ) >( );

	if ( !g_interfaces.m_engine_client->is_in_game( ) )
		return original( thisptr, edx, x, y );

	if ( g_movement.m_edgebug_data.m_will_edgebug && *x != 0 ) {
		const float factor =
			100.f /
			( static_cast< float >( std::pow< int, float >( g_movement.m_edgebug_data.m_ticks_to_stop,
		                                                    ( g_movement.m_edgebug_data.m_last_tick + g_movement.m_edgebug_data.m_ticks_to_stop -
		                                                      g_interfaces.m_global_vars_base->m_tick_count ) /
		                                                        *x ) ) +
		      100.f + ( GET_VARIABLE( g_variables.m_edge_bug_lock_amt, float ) * g_movement.m_edgebug_data.m_saved_mousedx ) );

		if ( !std::isnan( factor ) )
			*x *= factor;

		if ( *y != 0 )
			*y *= 1.f - ( 0.1f * GET_VARIABLE( g_variables.m_edge_bug_lock_amt, float ) * 10.f );
	}

	original( thisptr, edx, x, y );
}
