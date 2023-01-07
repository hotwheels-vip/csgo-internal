#include "frame_stage_notify.h"

void __fastcall hooks::frame_stage_notify::frame_stage_notify_detour( void* ecx, void* edx, sdk::frame_stage stage )
{
	hooks::frame_stage_notify_hook.call_original< void >( ecx, edx, stage );

	switch ( stage ) {
		using enum sdk::frame_stage;
	case NET_UPDATE_END:
		g_entity_list.update( );

		for ( auto event = g_interfaces.client_state->events; event; event = event->next )
			event->fire_delay = 0.f;

		g_interfaces.engine->fire_events( );

		for ( auto& player_info : g_entity_list.players ) {
			auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( player_info.m_index );

			if ( !player_info.m_valid || !player || !player->is_enemy( g_ctx.local ) )
				continue;

			g_ctx.updating_animations = true;
			// player->update_client_side_animation( );
			// g_ctx.updating_animations = false;

			auto var_mapping = player->get_var_map( );

			for ( int iterator = 0; iterator < var_mapping->interpolated_entries; iterator++ )
				var_mapping->entries[ iterator ].needs_to_interpolate = false;
		}

		g_lagcomp.update( );

		if ( g_ctx.local ) {
			sdk::datatypes::c_utl_vector< sdk::sound_info > sounds{ };

			g_interfaces.engine_sound->get_active_sounds( sounds );

			for ( int iterator = 0; iterator < sounds.count( ); iterator++ ) {
				auto& info = sounds[ iterator ];

				if ( !info.origin )
					return;

				auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( info.sound_source );

				if ( !player )
					return;

				if ( !player->is_player( ) )
					return;

				if ( !info.origin )
					return;

				math::vec3 sound_origin = *info.origin;
				math::vec3 sound_floor  = { sound_origin.x, sound_origin.y, sound_origin.z - 9000 };

				sdk::ray_t ray;
				sdk::c_game_trace trace;
				sdk::c_trace_filter filter;

				filter.skip = player;

				ray.init( sound_origin, sound_floor );
				g_interfaces.engine_trace->trace_ray( ray, sdk::masks::MASK_ALL, &filter, &trace );

				sound_origin = trace.end_pos;

				g_entity_list.players[ player->entity_index( ) ].m_dormant_info.m_last_position = sound_origin;
				g_entity_list.players[ player->entity_index( ) ].m_dormant_info.m_found_tick    = g_interfaces.globals->tick_count;
			}
		}
		break;
	}
}
