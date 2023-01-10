#include "../../convars/convars.h"
#include "../../features/movement/movement.h"
#include "../../features/prediction/prediction.h"
#include "../hooks.h"

bool __stdcall n_detoured_functions::create_move( float input_sample_time, c_user_cmd* cmd )
{
	static auto original = hooks.create_move.get_original< bool( __stdcall* )( float, c_user_cmd* ) >( );

	if ( !cmd || !cmd->m_command_number )
		return original( input_sample_time, cmd );

	// trust me.
	if ( !interfaces.m_engine->is_in_game( ) )
		return true;

	globals.m_cmd = cmd;

	if ( !memory.m_client_state /*|| interfaces.m_engine->is_playing_demo( )*/ )
		return original( input_sample_time, cmd );

	globals.m_old_view_point = cmd->m_view_point;

	if ( const bool valid = memory.m_client_state->m_delta_tick > 0; valid )
		interfaces.m_prediction->update( memory.m_client_state->m_delta_tick, valid, memory.m_client_state->m_last_command_ack,
		                                 memory.m_client_state->m_last_outgoing_command + memory.m_client_state->m_choked_commands );

	[ & ]( ) {
		if ( !globals.m_local || !globals.m_local->is_alive( ) || !globals.m_cmd ) {
			movement.m_edgebug_data.m_ticks_to_stop = 0;
			movement.m_edgebug_data.m_last_tick     = 0;
			return;
		}

		movement.on_create_move_pre( );

		prediction.begin( );
		prediction.end( );

		movement.on_create_move_post( );
	}( );

	globals.m_cmd->m_view_point.normalize( );
	globals.m_cmd->m_view_point.clamp( );
	globals.m_last_tick_yaw = cmd->m_view_point.m_y;

	/*unsigned int* frame;
	__asm mov frame, ebp;
	bool& send_packet = *reinterpret_cast< bool* >( *frame - 0x1C );*/

	return false;
}
