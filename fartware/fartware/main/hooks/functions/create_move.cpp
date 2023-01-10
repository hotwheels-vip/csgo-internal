#include "../../convars/convars.h"
#include "../../features/movement/movement.h"
#include "../../features/prediction/prediction.h"
#include "../hooks.h"

bool __stdcall n_detoured_functions::create_move( float input_sample_time, c_user_cmd* cmd )
{
	static auto original = hooks.create_move.get_original< decltype( &n_detoured_functions::create_move ) >( );

	original( input_sample_time, cmd );

	if ( !cmd || !cmd->m_command_number )
		return original( input_sample_time, cmd );

	globals.m_cmd            = cmd;
	globals.m_old_view_point = cmd->m_view_point;

	// returning true here calls set local view angles and fixes air stutter.
	if ( !interfaces.m_engine->is_in_game( ) )
		return true;

	if ( !globals.m_local )
		return original( input_sample_time, cmd );

	if ( const bool valid = memory.m_client_state->m_delta_tick > 0; valid )
		interfaces.m_prediction->update( memory.m_client_state->m_delta_tick, valid, memory.m_client_state->m_last_command_ack,
		                                 memory.m_client_state->m_last_outgoing_command + memory.m_client_state->m_choked_commands );

	[ & ]( ) {
		if ( !globals.m_local->is_alive( ) ) {
			movement.edgebug_data.ticks_to_stop = 0;
			movement.edgebug_data.last_tick     = 0;
			return;
		}

		movement.on_create_move_pre( );

		prediction.begin( );
		prediction.end( );

		movement.on_create_move_post( );
	}( );

	cmd->m_view_point.normalize( );
	cmd->m_view_point.clamp( );

	unsigned int* frame;
	__asm mov frame, ebp;
	bool& send_packet = *reinterpret_cast< bool* >( *frame - 0x1C );

	return false;
}
