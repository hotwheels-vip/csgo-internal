#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../hacks/prediction/prediction.h"

void __stdcall create_move( int sequence_number, float input_sample_frametime, bool is_active, bool& send_packet )
{
	static auto original = g_hooks.m_create_move_proxy.get_original< decltype( &n_detoured_functions::create_move_proxy ) >( );
	original( g_interfaces.m_base_client, 0, sequence_number, input_sample_frametime, is_active );

	c_user_cmd* cmd                   = g_interfaces.m_input->get_user_cmd( sequence_number );
	c_verified_user_cmd* verified_cmd = g_interfaces.m_input->get_verified_cmd( sequence_number );

	if ( !cmd || !verified_cmd )
		return;

	g_globals.cmd = cmd;

	if ( const bool valid = g_interfaces.m_client_state->m_delta_tick > 0; valid )
		g_interfaces.m_prediction->update( g_interfaces.m_client_state->m_delta_tick, valid, g_interfaces.m_client_state->m_last_command_ack,
		                                   g_interfaces.m_client_state->m_last_outgoing_command + g_interfaces.m_client_state->m_choked_commands );

	const auto local = g_interfaces.m_client_entity_list->get< c_base_entity >( g_interfaces.m_engine_client->get_local_player( ) );

	g_globals.local = local;

	[ & ]( ) {
		if ( !local || !local->is_alive( ) || !g_interfaces.m_engine_client->is_connected( ) )
			return;

		g_prediction.begin( local, cmd );
		g_prediction.end( local );
	}( );

	cmd->m_view_point.normalize( );
	cmd->m_view_point.clamp( );

	verified_cmd->m_user_cmd = *cmd;
	verified_cmd->m_hash_crc = cmd->get_check_sum( );
}

__declspec( naked ) void __fastcall n_detoured_functions::create_move_proxy( [[maybe_unused]] void* ecx, [[maybe_unused]] void* edx,
                                                                             [[maybe_unused]] int sequence_number,
                                                                             [[maybe_unused]] float input_sample_frametime,
                                                                             [[maybe_unused]] bool is_active )
{
	__asm
	{
		push	ebp
		mov		ebp, esp; 
		push	ebx; 
		push	esp; 
		push	dword ptr[is_active]; 
		push	dword ptr[input_sample_frametime]; 
		push	dword ptr[sequence_number];
		call	create_move
		pop		ebx
		pop		ebp
		retn	0Ch
	}
}
