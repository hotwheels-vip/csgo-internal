#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../hacks/animations/animations.h"
#include "../../hacks/debug/debug.h"
#include "../../hacks/entity_cache/entity_cache.h"
#include "../../hacks/movement/movement.h"
#include "../../hacks/visuals/edicts/edicts.h"

void __fastcall n_detoured_functions::frame_stage_notify( void* ecx, void* edx, int stage )
{
	static auto original = g_hooks.m_frame_stage_notify.get_original< decltype( &n_detoured_functions::frame_stage_notify ) >( );

	if ( !g_interfaces.m_engine_client->is_connected_safe( ) ) {
		g_edicts.reset( );
		g_lagcomp.clear_incoming_sequences( );
		return original( ecx, edx, stage );
	}

	if ( !g_ctx.m_local )
		return original( ecx, edx, stage );

	if ( !g_ctx.m_local->is_alive( ) || g_ctx.m_local->get_observer_mode( ) != e_obs_mode::OBS_MODE_NONE )
		g_edicts.reset( );

	g_edicts.on_frame_stage_notify( stage );
	g_movement.on_frame_stage_notify( stage );
	g_animations.on_frame_stage_notify( stage );
#ifdef _DEBUG
	g_debugger.on_frame_stage_notify( stage );
#endif
	if ( stage == net_update_end ) {
		for ( auto m_event = g_interfaces.m_client_state->m_events; m_event; m_event = m_event->m_next ) {
			if ( m_event->m_class_id )
				break;

			m_event->m_fire_delay = 0.f;
		}

		g_interfaces.m_engine_client->fire_events( );

		[ & ]( ) {
		// this will only work in debug build.
#ifdef _DEBUG
			if ( !GET_VARIABLE( g_variables.m_disable_interp, bool ) )
				return;
#else
			return;
#endif

			if ( g_ctx.m_local->get_observer_mode( ) != e_obs_mode::OBS_MODE_NONE )
				return;

			g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
				if ( !entity->is_valid_enemy( ) )
					return;

				const auto var_map = entity->get_var_map( );
				if ( !var_map )
					return;

				if ( var_map->interpolated_entries > 20108 )
					return;

				for ( int i = 0; i < var_map->interpolated_entries; i++ )
					var_map->entries[ i ].needs_to_interpolate = false;
			} );
		}( );

		g_lagcomp.on_frame_stage_notify( );
	}

	original( ecx, edx, stage );
}
