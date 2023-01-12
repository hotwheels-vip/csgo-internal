#include "../../convars/convars.h"
#include "../../features/movement/movement.h"
#include "../../features/prediction/prediction.h"
#include "../hooks.h"

// bool __stdcall n_detoured_functions::create_move( float input_sample_time, c_user_cmd* cmd )
//{
//	static auto original = hooks.create_move.get_original< bool( __stdcall* )( float, c_user_cmd* ) >( );
//
//	if ( !cmd || !cmd->m_command_number )
//		return original( input_sample_time, cmd );
//
//	// trust me.
//	if ( !interfaces.m_engine->is_in_game( ) )
//		return true;
//
//	globals.m_cmd = cmd;
//
//	if ( !memory.m_client_state /*|| interfaces.m_engine->is_playing_demo( )*/ )
//		return original( input_sample_time, cmd );
//
//	globals.m_old_view_point = cmd->m_view_point;
//
//	if ( const bool valid = memory.m_client_state->m_delta_tick > 0; valid )
//		interfaces.m_prediction->update( memory.m_client_state->m_delta_tick, valid, memory.m_client_state->m_last_command_ack,
//		                                 memory.m_client_state->m_last_outgoing_command + memory.m_client_state->m_choked_commands );
//
//	[ & ]( ) {
//		if ( !globals.m_local || !globals.m_local->is_alive( ) || !globals.m_cmd ) {
//			movement.m_edgebug_data.m_ticks_to_stop = 0;
//			movement.m_edgebug_data.m_last_tick     = 0;
//			return;
//		}
//
//		movement.on_create_move_pre( );
//
//		prediction.begin( globals.m_cmd );
//		prediction.end( );
//
//		movement.on_create_move_post( );
//	}( );
//
//	globals.m_cmd->m_view_point.normalize( );
//	globals.m_cmd->m_view_point.clamp( );
//	globals.m_last_tick_yaw = cmd->m_view_point.m_y;
//
//	/*unsigned int* frame;
//	__asm mov frame, ebp;
//	bool& send_packet = *reinterpret_cast< bool* >( *frame - 0x1C );*/
//
//	return false;
// }
// bool __stdcall n_detoured_functions::create_move( float input_sample_time, c_user_cmd* cmd )
//{
//	static auto original = hooks.create_move.get_original< bool( __stdcall* )( float, c_user_cmd* ) >( );
//
//	if ( !cmd || !cmd->m_command_number )
//		return original( input_sample_time, cmd );
//
//	// trust me.
//	if ( !interfaces.m_engine->is_in_game( ) )
//		return true;
//
//	globals.m_cmd = cmd;
//
//	if ( !memory.m_client_state /*|| interfaces.m_engine->is_playing_demo( )*/ )
//		return original( input_sample_time, cmd );
//
//	globals.m_old_view_point = cmd->m_view_point;
//
//	if ( const bool valid = memory.m_client_state->m_delta_tick > 0; valid )
//		interfaces.m_prediction->update( memory.m_client_state->m_delta_tick, valid, memory.m_client_state->m_last_command_ack,
//		                                 memory.m_client_state->m_last_outgoing_command + memory.m_client_state->m_choked_commands );
//
//	[ & ]( ) {
//		if ( !globals.m_local || !globals.m_local->is_alive( ) || !globals.m_cmd ) {
//			movement.m_edgebug_data.m_ticks_to_stop = 0;
//			movement.m_edgebug_data.m_last_tick     = 0;
//			return;
//		}
//
//		movement.on_create_move_pre( );
//
//		prediction.begin( globals.m_cmd );
//		prediction.end( );
//
//		movement.on_create_move_post( );
//	}( );
//
//	globals.m_cmd->m_view_point.normalize( );
//	globals.m_cmd->m_view_point.clamp( );
//	globals.m_last_tick_yaw = cmd->m_view_point.m_y;
//
//	/*unsigned int* frame;
//	__asm mov frame, ebp;
//	bool& send_packet = *reinterpret_cast< bool* >( *frame - 0x1C );*/
//
//	return false;
// }

static void __stdcall create_move( int sequence_number, float input_sample_frametime, bool is_active, bool& send_packet )
{
	static auto original = hooks.create_move_proxy.get_original< decltype( &n_detoured_functions::create_move_proxy ) >( );

	original( interfaces.m_client, 0, sequence_number, input_sample_frametime, is_active );

	c_user_cmd* cmd                   = memory.m_input->get_user_cmd( sequence_number );
	c_verified_user_cmd* verified_cmd = memory.m_input->get_verified_cmd( sequence_number );

	if ( !cmd || !verified_cmd || !is_active )
		return;

	globals.m_cmd = cmd;

	globals.m_old_view_point = globals.m_cmd->m_view_point;

	if ( const bool valid = memory.m_client_state->m_delta_tick > 0; valid )
		interfaces.m_prediction->update( memory.m_client_state->m_delta_tick, valid, memory.m_client_state->m_last_command_ack,
		                                 memory.m_client_state->m_last_outgoing_command + memory.m_client_state->m_choked_commands );

	[ & ]( ) {
		if ( !globals.m_local || !globals.m_local->is_alive( ) || !globals.m_cmd ) {
			movement.m_edgebug_data.m_ticks_to_stop = 0;
			movement.m_edgebug_data.m_last_tick     = 0;

			edgebug_g::will_edgebug      = true;
			edgebug_g::will_fail         = false;
			edgebug_g::edgebug_ticks     = 0;
			edgebug_g::edgebug_tickcount = 0;
			edgebug_g::edgebug_method    = edgebug_type::standing;
			return;
		}

		movement.on_create_move_pre( );

		prediction.begin( globals.m_cmd );
		prediction.end( );

		movement.on_create_move_post( );

		movement.edgebug( globals.m_cmd );
	}( );

	globals.m_cmd->m_view_point.normalize( );
	globals.m_cmd->m_view_point.clamp( );
	globals.m_last_tick_yaw = globals.m_cmd->m_view_point.m_y;

	verified_cmd->m_user_cmd = *globals.m_cmd;
	verified_cmd->m_hash_crc = globals.m_cmd->get_checksum( );
}

__declspec( naked ) void __fastcall n_detoured_functions::create_move_proxy( [[maybe_unused]] void* thisptr, [[maybe_unused]] int edx,
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
