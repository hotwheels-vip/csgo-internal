#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

// this is scatterd around source. please fix.
#undef max

int __fastcall n_detoured_functions::send_datagram( c_net_channel* net_channel, int edx, bf_write* datagram )
{
	static auto original = g_hooks.m_send_datagram.get_original< decltype( &n_detoured_functions::send_datagram ) >( );

	auto nci          = g_interfaces.m_engine_client->get_net_channel_info( );
	static auto unlag = g_convars[ HASH_BT( "sv_maxunlag" ) ];

	if ( !g_interfaces.m_engine_client->is_connected_safe( ) || !GET_VARIABLE( g_variables.m_backtrack_extend, bool ) || !unlag || datagram || !nci )
		return original( net_channel, edx, datagram );

	const int old_in_reliable_state = net_channel->m_in_reliable_state;
	const int old_in_sequence       = net_channel->m_in_sequence_nr;

	const float max_latency =
		std::max( 0.f, g_convars[ HASH_BT( "sv_maxunlag" ) ]->get_float( ) ) - nci->get_latency( FLOW_OUTGOING );

	g_lagcomp.add_latency_to_net_channel( net_channel, max_latency );

	const int result = original( net_channel, edx, datagram );

	net_channel->m_in_reliable_state = old_in_reliable_state;
	net_channel->m_in_sequence_nr    = old_in_sequence;

	return result;
}
