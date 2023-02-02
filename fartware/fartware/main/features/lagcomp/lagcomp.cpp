#include "lagcomp.h"
#include "../../convars/convars.h"
#include "../../includes.h"
#include "../entities/entities.h"

/*leaving as note*/

/*
ignore bones
iirc these are like 2k18 offsets & sigs
This should fix pred errors whilst lagging
& help with packet send time

https://pastebin.com/G3tHYtb0

https://pastebin.com/uBmsSwdQ

https://pastebin.com/u25RVqLw
*/

float lagcomp_t::lerp_time( )
{
	static auto *cl_updaterate              = convars.find( fnv1a::hash_const( "sv_maxupdaterate" ) ),
				*cl_interp_ratio            = convars.find( fnv1a::hash_const( "cl_interp_ratio" ) ),
				*cl_interp                  = convars.find( fnv1a::hash_const( "cl_interp" ) ),
				*sv_client_min_interp_ratio = convars.find( fnv1a::hash_const( "sv_client_min_interp_ratio" ) ),
				*sv_client_max_interp_ratio = convars.find( fnv1a::hash_const( "sv_client_max_interp_ratio" ) );

	static auto interp_ratio = std::max( cl_interp_ratio->get_float( ), 1.f );

	interp_ratio = std::clamp( interp_ratio, sv_client_min_interp_ratio->get_float( ), sv_client_max_interp_ratio->get_float( ) );

	return std::max( cl_interp->get_float( ), interp_ratio / cl_updaterate->get_int( ) );
}

bool lagcomp_t::is_valid( lagcomp_t::record rec )
{
	auto net_channel = interfaces.m_engine->get_net_channel_info( );

	if ( !net_channel )
		return false;

	static auto sv_maxunlag = convars.find( fnv1a::hash_const( "sv_maxunlag" ) );

	if ( !sv_maxunlag )
		return false;

	float correct = 0.f;

	correct += net_channel->get_latency( FLOW_OUTGOING );
	correct += net_channel->get_latency( FLOW_INCOMING );
	correct += lagcomp.lerp_time( );

	correct = std::clamp( correct, 0.f, sv_maxunlag->get_float( ) );

	const float delta_correct = std::fabsf( correct - ( memory.m_globals->m_current_time - rec.simulation_time ) );

	// TODO: configable
	return delta_correct <= .2f;
}

int lagcomp_t::max_ticks( )
{
	// this is gonna be used later for extended backtracking
	const int extra_ticks = 0;

	return mathematics.time_to_ticks( convars.find( fnv1a::hash_const( "sv_maxunlag" ) )->get_float( ) ) + extra_ticks;
}

void lagcomp_t::update( )
{
	if ( !GET_CONFIG_BOOL( variables.m_aimbot.m_backtrack_enabled ) ) {
		// for ( std::size_t i = 0; i < heap_records.size( ); i++ ) {
		//	if ( heap_records[ i ] ) {
		//		delete[] heap_records[ i ];
		//		heap_records[ i ] = NULL;
		//	}
		// }

		return;
	}

	for ( int i = 1; i < 64; i++ ) {
		auto player = reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity( i ) );
		if ( !player->is_valid( ) ) {
			if ( heap_records[ i ] ) {
				delete[] heap_records[ i ];
				heap_records[ i ] = NULL;
			}

			continue;
		}

		auto& current_heap_iterator = heap_iterator[ player->index( ) ];

		if ( !heap_records[ i ] )
			heap_records[ i ] = new record[ lagcomp.max_ticks( ) ];

		auto& current_record = heap_records[ i ][ current_heap_iterator ];

		current_record.abs_origin      = player->abs_origin( );
		current_record.eye_position    = player->eye_position( );
		current_record.simulation_time = player->simulation_time( );
		current_record.valid           = is_valid( current_record );
		current_record.player          = player;
		player->setup_bones( current_record.bone_matrix, 128, 256, 0.f );

		current_heap_iterator++;

		if ( current_heap_iterator >= lagcomp.max_ticks( ) )
			current_heap_iterator = 0;
	}
}

// grabs oldest avaiable record
std::optional< lagcomp_t::record > oldest_record( c_base_entity* player )
{
	if ( lagcomp.heap_records[ player->index( ) ] ) {
		int last_valid_heap_record   = 0;
		float lowest_simulation_time = FLT_MAX;

		for ( int current_heap_iterator = 0; current_heap_iterator < lagcomp.max_ticks( ); current_heap_iterator++ ) {
			lagcomp_t::record* current_record = &lagcomp.heap_records[ player->index( ) ][ current_heap_iterator ];

			if ( !current_record )
				continue;

			if ( current_record->simulation_time > lowest_simulation_time )
				continue;

			last_valid_heap_record = current_heap_iterator;
			lowest_simulation_time = current_record->simulation_time;
		}
		return std::make_optional( lagcomp.heap_records[ player->index( ) ][ last_valid_heap_record ] );
	}

	return std::nullopt;
}

void lagcomp_t::backtrack_player( c_base_entity* player )
{
	static auto recoil_scale = convars.find( fnv1a::hash_const( "weapon_recoil_scale" ) );

	auto entity_index = player->index( );

	if ( !heap_records[ entity_index ] )
		return;

	float closest_fov = std::numeric_limits< float >::max( );
	record* closest_record{ };

	c_angle player_angles{ };
	interfaces.m_engine->get_view_angles( player_angles );

	const auto eye_position = globals.m_local->eye_position( );

	for ( int current_heap_iterator = 0; current_heap_iterator < lagcomp.max_ticks( ); current_heap_iterator++ ) {
		auto current_record = &heap_records[ entity_index ][ current_heap_iterator ];

		if ( !current_record )
			continue;
		c_vector head_position = player->get_bone_position( e_hitgroup::hitgroup_head );

		c_angle delta = mathematics.calculate_angle( eye_position, head_position );
		delta -= ( globals.m_local->aim_punch_angle( ) * recoil_scale->get_float( ) );
		auto record_fov = mathematics.calculate_fov( player_angles, delta );

		if ( record_fov < closest_fov ) {
			closest_fov    = record_fov;
			closest_record = current_record;
		}
	}

	globals.m_record = closest_record;

	if ( !closest_record )
		return;

	// https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/server/player_lagcompensation.cpp#L287

	globals.m_cmd->m_tick_count = mathematics.time_to_ticks( closest_record->simulation_time + lerp_time( ) );
}

void lagcomp_t::backtrack_player( record* heap_record )
{
	globals.m_record = heap_record;

	if ( !heap_record )
		return;

	globals.m_cmd->m_tick_count = mathematics.time_to_ticks( heap_record->simulation_time + lerp_time( ) );
}
