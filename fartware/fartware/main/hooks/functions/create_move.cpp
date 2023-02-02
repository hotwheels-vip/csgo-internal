#include "../../convars/convars.h"
#include "../../features/aimbot/aimbot.h"
#include "../../features/lagcomp/lagcomp.h"
#include "../../features/misc/misc.h"
#include "../../features/movement/indicators/indicators.h"
#include "../../features/movement/movement.h"
#include "../../features/prediction/prediction.h"
#include "../hooks.h"

void __stdcall create_move( int sequence_number, float input_sample_frametime, bool is_active, bool& send_packet )
{
	static auto original = hooks.create_move_proxy.get_original< decltype( &n_detoured_functions::create_move_proxy ) >( );

	original( interfaces.m_client, 0, sequence_number, input_sample_frametime, is_active );

	c_user_cmd* cmd                   = memory.m_input->get_user_cmd( sequence_number );
	c_verified_user_cmd* verified_cmd = memory.m_input->get_verified_cmd( sequence_number );

	if ( !cmd || !verified_cmd )
		return;

	globals.m_cmd = cmd;

	globals.m_old_view_point = globals.m_cmd->m_view_point;

	if ( const bool valid = memory.m_client_state->m_delta_tick > 0; valid )
		interfaces.m_prediction->update( memory.m_client_state->m_delta_tick, valid, memory.m_client_state->m_last_command_ack,
		                                 memory.m_client_state->m_last_outgoing_command + memory.m_client_state->m_choked_commands );

	[ & ]( ) {
		// yes this is supposed to be here
		indicators.on_create_move_pre( );

		if ( !globals.m_local || !globals.m_local->is_alive( ) || !globals.m_cmd ||
		     globals.m_local->get_observer_mode( ) == e_obs_mode::obs_mode_deathcam ) {
			movement.m_edgebug_data.reset( );
			movement.m_pixelsurf_data.reset( );
			movement.m_autoduck_data.reset( );
			indicators.m_detection.reset( );

			globals.m_record = nullptr;
			return;
		}

		misc.on_create_move_pre( );
		movement.on_create_move_pre( );

		prediction.begin( globals.m_cmd );
		prediction.end( );

		indicators.on_create_move_post( );
		movement.on_create_move_post( );
		aimbot.on_create_move_post( );
	}( );

	globals.m_cmd->m_view_point.normalize( );
	globals.m_cmd->m_view_point.clamp( );
	globals.m_last_tick_yaw = globals.m_cmd->m_view_point.m_y;

	verified_cmd->m_user_cmd = *cmd;
	verified_cmd->m_hash_crc = cmd->get_checksum( );
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
