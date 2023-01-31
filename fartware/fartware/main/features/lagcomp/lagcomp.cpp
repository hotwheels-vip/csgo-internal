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

	float correct = 0.f;

	correct += net_channel->get_latency( FLOW_OUTGOING );
	correct += net_channel->get_latency( FLOW_INCOMING );
	correct += lagcomp.lerp_time( );

	correct = std::clamp( correct, 0.f, convars.find( fnv1a::hash_const( "sv_maxunlag" ) )->get_float( ) );

	const float delta_correct = std::fabsf( correct - ( memory.m_globals->m_current_time - rec.sim_time ) );

	// TODO: configable
	return delta_correct <= .2f;
}

// LIGA CODE FUCK YOU

void lagcomp_t::update( )
{
	// TODO, to make this function work:
	//  add setup bones function to cbase entity
	//  finish it, duh
	//  call it (not calling rn cause this is useless
	//  add checkbox in menu

	const bool backtrack_enabled = GET_CONFIG_BOOL( variables.m_aimbot.m_backtrack_enabled );

	// tickrate
	const auto max_allocation = static_cast< int >( 1.f / memory.m_globals->m_interval_per_tick );

	entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
		if ( !entity || !entity->is_alive( ) || entity->is_dormant( ) )
			return;

		const auto index = entity->index( );

		const auto& record_list = records[ index ];

		if ( !record_list )
			return;

		for ( int i = 0; i < max_allocation; i++ ) {
			auto& record = record_list[ i ];

			if ( record.player_index != index && record.player_index != -1 ) {
				delete[] records[ index ];

				records[ index ] = nullptr;

				break;
			}

			record.valid = backtrack_enabled ? is_valid( record ) : false;
		}
	} );

	// cant get entity index through entity cache, so we loop through it again. fuck

	for ( int i = 0; i < interfaces.m_client_entity_list->get_highest_entity_index( ); i++ ) {
		auto entity = reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity( i ) );

		if ( !entity || !entity->is_alive( ) || entity->is_dormant( ) ) {
			if ( records[ i ] ) {
				delete[] records[ i ];

				records[ i ] = nullptr;
			}

			continue;
		}

		auto record    = records[ i ];
		auto& location = record_location[ i ];

		if ( !record ) {
			records[ i ] = new lagcomp_t::record[ max_allocation ];

			location = 0;
			record   = records[ i ];
		}

		if ( location >= max_allocation )
			location = 0;

		lagcomp_t::record new_record{ };

		new_record.player_index = i;
		new_record.sim_time     = entity->simulation_time( );
		new_record.valid        = is_valid( new_record );

		entity->client_renderable( )->setup_bones( new_record.matrix, 128, 0x0007FF00, 0.f );

		memcpy( &record[ location ], &new_record, sizeof( lagcomp_t::record ) );

		location++;
	}
}
