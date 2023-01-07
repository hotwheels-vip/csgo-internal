#include "../../convars/convars.h"
#include "../../features/movement/movement.h"
#include "../../features/prediction/prediction.h"
#include "../hooks.h"

static void __stdcall create_move( int sequence_number, float input_sample_frametime, bool is_active, bool& send_packet )
{
	static auto original = hooks.create_move_proxy.get_original< decltype( &n_detoured_functions::create_move_proxy ) >( );

	original( interfaces.m_client, 0, sequence_number, input_sample_frametime, is_active );

	c_user_cmd* cmd                   = memory.m_input->get_user_cmd( sequence_number );
	c_verified_user_cmd* verified_cmd = memory.m_input->get_verified_cmd( sequence_number );

	if ( !cmd || !verified_cmd || !is_active )
		return;

	globals.m_cmd = cmd;

	[ & ]( ) {
		if ( !globals.m_local || !globals.m_local->is_alive( ) || !globals.m_cmd )
			return;

		movement.on_create_move_pre( );

		prediction.begin( );
		prediction.end( );

		movement.on_create_move_post( );
	}( );

	cmd->m_view_point.normalize( );
	cmd->m_view_point.clamp( );

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