#include "../../features/entities/avatars.h"
#include "../../features/entities/entities.h"
#include "../../features/movement/movement.h"
#include "../../features/visuals/weather/weather.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::frame_stage_notify( void* thisptr, int edx, e_client_frame_stage stage )
{
	static auto original = hooks.frame_stage_notify.get_original< decltype( &n_detoured_functions::frame_stage_notify ) >( );

	if ( !interfaces.m_engine->is_in_game( ) ) {
		weather.reset( );
		return original( thisptr, edx, stage );
	}

	globals.m_local =
		reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity( interfaces.m_engine->get_local_player( ) ) );

	if ( !globals.m_local )
		return original( thisptr, edx, stage );

	if ( !globals.m_local->is_alive( ) )
		weather.reset( );

	switch ( stage ) {
	case e_client_frame_stage::start:
		globals.m_view_matrix = interfaces.m_engine->world_to_screen_matrix( );

		movement.handle_edgebug_view_point( );
		break;
	};

	[ & ]( ) {
		entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
			if ( !entity || entity->is_dormant( ) || globals.m_local == entity || entity->team( ) == globals.m_local->team( ) )
				return;

			if ( stage == e_client_frame_stage::net_update_postdataupdate_end )
				for ( unsigned int iterator = 0; iterator < 5; iterator++ )
					entity->player_patch_econ_indices( )[ iterator ] = 0;
			else if ( stage == e_client_frame_stage::net_update_end ) {
				const auto var_map = entity->get_var_map( );
				if ( !var_map )
					return;

				if ( var_map->interpolated_entries > 20108 )
					return;

				for ( int i = 0; i < var_map->interpolated_entries; i++ )
					var_map->entries[ i ].needs_to_interpolate = false;
			}
		} ); /* rega r d s                                           */
	}( );

	weather.on_frame_stage_notify( stage );

	original( thisptr, edx, stage );
}
