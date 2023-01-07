#include "lagcomp.h"

#include "../../../game/sdk/enums/flow.h"

#include "../../../globals/ctx/ctx.h"

#include "../../../utils/convars/convars.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../../logging/logging.h"

bool lagcomp::impl::is_valid( record heap_record )
{
	// sanity check
	auto m_nci = g_interfaces.engine->get_net_channel_info( );
	if ( !m_nci )
		return false;

	float correct = 0.f;

	static auto unlag_pointer = g_convars[ _( "sv_maxunlag" ) ];
	auto sv_maxunlag          = unlag_pointer->get_float( );

	correct += m_nci->get_latency( sdk::flow::FLOW_OUTGOING );
	correct += lerp_time( );

	correct = std::clamp( correct, 0.f, sv_maxunlag );

	float delta_correct = std::fabsf( correct - ( g_interfaces.globals->current_time - heap_record.simulation_time ) );

	return delta_correct <= .2f;
}

float lagcomp::impl::lerp_time( )
{
	static sdk::con_var *cl_updaterate = g_convars[ _( "cl_updaterate" ) ], *cl_interp_ratio = g_convars[ _( "cl_interp_ratio" ) ],
						*cl_interp = g_convars[ _( "cl_interp" ) ], *sv_client_min_interp_ratio = g_convars[ _( "sv_client_min_interp_ratio" ) ],
						*sv_client_max_interp_ratio = g_convars[ _( "sv_client_max_interp_ratio" ) ];

	static auto interp_ratio = std::max( cl_interp_ratio->get_float( ), 1.f );

	interp_ratio = std::clamp( interp_ratio, sv_client_min_interp_ratio->get_float( ), sv_client_max_interp_ratio->get_float( ) );

	return std::max( cl_interp->get_float( ), interp_ratio / cl_updaterate->get_int( ) );
}

void lagcomp::impl::update( )
{
	static auto unlag_pointer = g_convars[ _( "sv_maxunlag" ) ];
	auto sv_maxunlag_ticks    = sdk::time_to_ticks( unlag_pointer->get_float( ) );

	for ( auto& player_info : g_entity_list.players ) {
		auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( player_info.m_index );

		if ( !player_info.m_valid || !player || !player->is_enemy( g_ctx.local ) ) {
			if ( heap_records[ player_info.m_index ] ) {
				delete[] heap_records[ player_info.m_index ];
				heap_records[ player_info.m_index ] = NULL;
			}

			continue;
		}

		auto& current_heap_iterator = heap_iterator[ player->entity_index( ) ];

		if ( !heap_records[ player_info.m_index ] )
			heap_records[ player_info.m_index ] = new record[ sv_maxunlag_ticks ];

		auto& current_record = heap_records[ player_info.m_index ][ current_heap_iterator ];

		current_record.abs_origin      = player->get_abs_angles( );
		current_record.eye_position    = player->eye_position( );
		current_record.simulation_time = player->simulation_time( );
		current_record.valid           = is_valid( current_record );
		current_record.player          = player;
		player->setup_bones( current_record.bone_matrix, 128, 256, 0.f );

		current_heap_iterator++;

		if ( current_heap_iterator >= sv_maxunlag_ticks )
			current_heap_iterator = 0;
	}
}

void lagcomp::impl::backtrack_player( record* heap_record )
{
	g_ctx.record = heap_record;

	if ( !heap_record )
		return;

	g_ctx.cmd->tick_count = sdk::time_to_ticks( heap_record->simulation_time + lerp_time( ) );
}

void lagcomp::impl::backtrack_player( sdk::c_cs_player* player )
{
	static auto unlag_pointer = g_convars[ _( "sv_maxunlag" ) ];
	auto sv_maxunlag_ticks    = sdk::time_to_ticks( unlag_pointer->get_float( ) );

	auto entity_index = player->entity_index( );

	auto closest_fov = FLT_MAX;
	record* closest_record{ };

	math::vec3 player_angles = g_interfaces.engine->get_view_angles( );

	if ( !heap_records[ entity_index ] )
		return;

	for ( int current_heap_iterator = 0; current_heap_iterator < sv_maxunlag_ticks; current_heap_iterator++ ) {
		auto current_record = &heap_records[ entity_index ][ current_heap_iterator ];

		if ( !current_record )
			continue;

		auto record_fov = math::get_fov( player_angles, g_ctx.local->eye_position( ), current_record->eye_position );

		if ( record_fov < closest_fov ) {
			closest_fov    = record_fov;
			closest_record = current_record;
		}
	}

	g_ctx.record = closest_record;

	if ( !closest_record )
		return;

	// https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/server/player_lagcompensation.cpp#L287
	// ty dalkr :3

	g_ctx.cmd->tick_count = sdk::time_to_ticks( closest_record->simulation_time + lerp_time( ) );
}
