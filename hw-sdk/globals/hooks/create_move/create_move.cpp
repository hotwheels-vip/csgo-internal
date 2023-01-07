#include <string.h>

#include "../../../hacks/features/aimbot/aimbot.h"
#include "../../../hacks/features/lagcomp/lagcomp.h"
#include "../../../hacks/features/movement/movement.h"
#include "../../../hacks/prediction/prediction.h"
#include "../../../utils/convars/convars.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../../../utils/keybinds/keybinds.h"
#include "../../../utils/particle_system/particle_system.h"
#include "../../ctx/ctx.h"
#include "../cl_move/cl_move.h"
#include "create_move.h"

void __stdcall create_move_function( int sequence_number, float input_sample_frametime, bool active, bool& send_packet )
{
	hooks::create_move_hook.call_original< void >( g_interfaces.client, nullptr, sequence_number, input_sample_frametime, active );

	auto command  = g_interfaces.input->get_user_cmd( sequence_number );
	auto verified = g_interfaces.input->get_verified_cmd( sequence_number );

	g_ctx.cmd = command;

	if ( !g_ctx.local || !command || !verified || !command->command_number )
		return;

	g_movement.pre_prediction.think( );

	g_prediction.update( );
	g_prediction.store_backup( );

	g_prediction.start( g_ctx.local );
	g_prediction.end( g_ctx.local );

	g_aimbot.run( );
	g_movement.movement_fix( command, g_prediction.backup_vars.view_angles );

	g_movement.post_prediction.think( );

	// if ( hooks::shifting_tb )
	//	send_packet = hooks::send_packet;

	verified->command  = *command;
	verified->checksum = command->checksum( );
}

__declspec( naked ) void __fastcall hooks::create_move::create_move_detour( sdk::i_client_dll* _this, void* edx, int sequence_number,
                                                                            float input_sample_frametime, bool active )
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx
		push esp
		push dword ptr[active]
		push dword ptr[input_sample_frametime]
		push dword ptr[sequence_number]
		call create_move_function
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}
