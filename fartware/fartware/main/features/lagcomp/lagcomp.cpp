#include "lagcomp.h"
#include "../../convars/convars.h"
#include "../../includes.h"

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
	//  make option in menu for backtrack(no need for customizable backtrack time range, useless just make it always max)

	auto net_channel = interfaces.m_engine->get_net_channel_info( );

	if ( !net_channel )
		return false;

	float correct = 0.f;

	correct += net_channel->get_latency( 0 );
	correct += lagcomp.lerp_time( );

	correct = std::clamp( correct, 0.f, convars.find( fnv1a::hash_const( "sv_maxunlag" ) )->get_float( ) );

	const float delta_correct = std::fabsf( correct - ( memory.m_globals->m_current_time - rec.sim_time ) );

	// TODO

	// return delta_correct <= static_cast< float >( *aimbot_lagcomp_time ) / 1000.f;

	return true;
}

void lagcomp_t::update( )
{
	// TODO, to make this function work:
	//  add setup bones function to cbase entity
	//  finish it, duh
	//  call it (not calling rn cause this is useless
	//  add checkbox in menu

	// tickrate
	const auto max_allocation = static_cast< int >( 1.f / memory.m_globals->m_interval_per_tick );
}
