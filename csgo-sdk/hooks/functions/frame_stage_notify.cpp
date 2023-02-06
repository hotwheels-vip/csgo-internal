#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::frame_stage_notify(void* ecx, void* edx, int stage)
{
	static auto original = g_hooks.m_frame_stage_notify.get_original< decltype( &n_detoured_functions::frame_stage_notify ) >( );

	if ( !g_interfaces.m_engine_client->is_in_game( ) )
		return original( ecx, edx, stage );

	const auto local = g_interfaces.m_client_entity_list->get< c_base_entity >( g_interfaces.m_engine_client->get_local_player( ) );
	if ( !local )
		return original( ecx, edx, stage );

	original( ecx, edx, stage );
}