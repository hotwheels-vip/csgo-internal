#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../hacks/entity_cache/entity_cache.h"
#include "../../hacks/movement/movement.h"
#include "../../hacks/visuals/edicts/edicts.h"

void __fastcall n_detoured_functions::frame_stage_notify( void* ecx, void* edx, int stage )
{
	static auto original = g_hooks.m_frame_stage_notify.get_original< decltype( &n_detoured_functions::frame_stage_notify ) >( );

	if ( !g_interfaces.m_engine_client->is_in_game( ) ) {
		g_edicts.reset( );
		return original( ecx, edx, stage );
	}

	if ( !g_ctx.m_local )
		return original( ecx, edx, stage );

	if ( !g_ctx.m_local->is_alive( ) )
		g_edicts.reset( );

	g_edicts.on_frame_stage_notify( stage );
	g_movement.on_frame_stage_notify( stage );

	[ & ]( ) {
		g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
			if ( !entity || entity->is_dormant( ) || g_ctx.m_local == entity || !g_ctx.m_local->is_enemy( entity ) )
				return;

			if ( stage == e_client_frame_stage::net_update_end ) {
				const auto var_map = entity->get_var_map( );
				if ( !var_map )
					return;

				if ( var_map->interpolated_entries > 20108 )
					return;

				for ( int i = 0; i < var_map->interpolated_entries; i++ )
					var_map->entries[ i ].needs_to_interpolate = false;
			}
		} );
	}( );

	original( ecx, edx, stage );
}
