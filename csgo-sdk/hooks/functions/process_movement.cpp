#include "../../game/sdk/classes/c_move_data.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::process_movement( void* thisptr, void* edx, c_base_entity* player, c_move_data* move_data )
{
	static auto original = g_hooks.m_process_movement.get_original< decltype( &n_detoured_functions::process_movement ) >( );

	// fix prediction errros when jumping
	// https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/game/shared/gamemovement.cpp#L5036
	move_data->m_game_code_moved_player = false;

	return original( thisptr, edx, player, move_data );
}
