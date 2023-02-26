#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../hacks/aimbot/aimbot.h"
#include "../../hacks/lagcomp/lagcomp.h"
#include "../../hacks/misc/misc.h"
#include "../../hacks/misc/scaleform/scaleform.h"
#include "../../hacks/movement/movement.h"
#include "../../hacks/prediction/prediction.h"

void __stdcall create_move( int sequence_number, float input_sample_frametime, bool is_active, bool& send_packet )
{
	static auto original = g_hooks.m_create_move_proxy.get_original< decltype( &n_detoured_functions::create_move_proxy ) >( );
	original( g_interfaces.m_base_client, 0, sequence_number, input_sample_frametime, is_active );

	c_user_cmd* cmd                   = g_interfaces.m_input->get_user_cmd( sequence_number );
	c_verified_user_cmd* verified_cmd = g_interfaces.m_input->get_verified_cmd( sequence_number );

	if ( !cmd || !verified_cmd || !cmd->m_command_number || cmd->m_tick_count == 0 )
		return;

	g_ctx.m_cmd = cmd;

	const auto local = g_interfaces.m_client_entity_list->get< c_base_entity >( g_interfaces.m_engine_client->get_local_player( ) );

	g_ctx.m_local = local;

	g_prediction.update( );

	g_scaleform.on_createmove( );

	[ & ]( ) {
		if ( !g_ctx.m_local || !g_ctx.m_local->is_alive( ) || !g_ctx.m_cmd || g_ctx.m_local->get_observer_mode( ) == 1 /*DEATH_CAM*/ ) {
			g_movement.m_edgebug_data.reset( );
			g_movement.m_pixelsurf_data.reset( );
			g_movement.m_autoduck_data.reset( );
			return;
		
		}
		g_movement.on_create_move_pre( );
		g_misc.on_create_move_pre( );

		g_prediction.begin( g_ctx.m_local, cmd );

		g_lagcomp.on_create_move_post( );
		g_aimbot.on_create_move_post( );

		g_prediction.end( g_ctx.m_local );

		g_movement.on_create_move_post( );

		g_console.print( std::vformat( "move_collide -> {:d}", std::make_format_args( g_ctx.m_local->get_move_collide( ) ) ).c_str( ) );
	}( );

	cmd->m_view_point.normalize( );
	cmd->m_view_point.clamp( );

	g_ctx.m_last_tick_yaw = cmd->m_view_point.m_y;

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
