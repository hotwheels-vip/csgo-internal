#include "prediction.h"
#include "../../globals.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../../utilities/input/input.h"
#include "../movement/movement.h"

void prediction_t::begin( c_user_cmd* cmd )
{
	if ( m_last_cmd )
		m_last_cmd->m_has_been_predicted ? prediction.m_correct_tick_base = globals.m_local->tick_base( ) : prediction.m_correct_tick_base++;

	m_last_cmd = cmd;

	*globals.m_local->current_command( ) = cmd;
	globals.m_local->last_command( )     = *cmd;

	if ( !prediction.m_prediction_random_seed || !prediction.m_prediction_player ) {
		prediction.m_prediction_random_seed =
			*reinterpret_cast< unsigned int** >( memory.m_modules[ e_module_names::client ].find_pattern( ( "A3 ? ? ? ? 66 ? ? 86" ) ) + 0x1 );

		prediction.m_prediction_player = *reinterpret_cast< c_base_entity*** >(
			memory.m_modules[ e_module_names::client ].find_pattern( ( "89 35 ? ? ? ? F3 0F 10 48 20" ) ) + 0x2 );
	}

	*prediction.m_prediction_random_seed = cmd->m_random_seed & 0x7FFFFFFF;
	*prediction.m_prediction_player      = globals.m_local;

	prediction.m_old_current_time = memory.m_globals->m_current_time;
	prediction.m_old_frame_time   = memory.m_globals->m_frame_time;
	prediction.m_old_tick_count   = memory.m_globals->m_tick_count;

	const int old_tick_base = globals.m_local->tick_base( );

	const bool old_is_first_prediction = interfaces.m_prediction->m_is_first_time_predicted;
	const bool old_in_prediction       = interfaces.m_prediction->m_in_prediction;

	memory.m_globals->m_current_time = mathematics.ticks_to_time( prediction.m_correct_tick_base );
	memory.m_globals->m_frame_time   = interfaces.m_prediction->m_engine_paused ? 0.f : memory.m_globals->m_interval_per_tick;
	memory.m_globals->m_tick_count   = globals.m_local->tick_base( );

	interfaces.m_prediction->m_is_first_time_predicted = false;
	interfaces.m_prediction->m_in_prediction           = true;

	cmd->m_buttons |= globals.m_local->button_forced( );
	cmd->m_buttons &= ~( globals.m_local->button_disabled( ) );

	interfaces.m_game_movement->start_track_prediction_errors( globals.m_local );

	if ( cmd->m_weapon_select ) {
		static auto active_weapon = reinterpret_cast< c_base_entity* >(
			interfaces.m_client_entity_list->get_client_entity_from_handle( globals.m_local->active_weapon_handle( ) ) );
		if ( active_weapon ) {
			static auto weapon_data = memory.m_weapon_system->get_weapon_data( active_weapon->item_definition_index( ) );
			if ( weapon_data )
				globals.m_local->select_item( weapon_data->m_weapon_name, cmd->m_weapon_sub_type );
		}
	}

	const int buttons         = cmd->m_buttons;
	const int local_buttons   = *globals.m_local->buttons( );
	const int buttons_changed = buttons ^ local_buttons;

	globals.m_local->button_last( )     = local_buttons;
	*globals.m_local->buttons( )        = buttons;
	globals.m_local->button_pressed( )  = buttons_changed & buttons;
	globals.m_local->button_released( ) = buttons_changed & ( ~buttons );

	interfaces.m_prediction->check_moving_ground( globals.m_local, memory.m_globals->m_frame_time );

	interfaces.m_prediction->set_local_view_angles( cmd->m_view_point );

	if ( globals.m_local->physics_run_think( 0 ) )
		globals.m_local->pre_think( );

	if ( int* next_think_tick = globals.m_local->next_think_tick( ); *next_think_tick > 0 && *next_think_tick <= prediction.m_correct_tick_base ) {
		*next_think_tick = -1;

		globals.m_local->set_next_think( 0 );

		globals.m_local->think( );
	}

	memory.m_move_helper->set_host( globals.m_local );

	interfaces.m_prediction->setup_move( globals.m_local, cmd, memory.m_move_helper, &prediction.m_move_data );

	movement.handle_move_data( );

	interfaces.m_game_movement->process_movement( globals.m_local, &prediction.m_move_data );

	interfaces.m_prediction->finish_move( globals.m_local, cmd, &prediction.m_move_data );

	memory.m_move_helper->process_impacts( );

	globals.m_local->post_think( );

	// globals.m_local->tick_base( ) = m_correct_tick_base;

	interfaces.m_prediction->m_in_prediction           = old_in_prediction;
	interfaces.m_prediction->m_is_first_time_predicted = old_is_first_prediction;
}

void prediction_t::end( ) const
{
	interfaces.m_game_movement->finish_track_prediction_errors( globals.m_local );

	memory.m_move_helper->set_host( nullptr );

	memory.m_globals->m_current_time = prediction.m_old_current_time;
	memory.m_globals->m_frame_time   = prediction.m_old_frame_time;
	memory.m_globals->m_tick_count   = prediction.m_old_tick_count;

	*globals.m_local->current_command( ) = nullptr;

	*prediction.m_prediction_random_seed = -1;

	*prediction.m_prediction_player = nullptr;

	interfaces.m_game_movement->reset( );
	// if ( interfaces.m_prediction->m_engine_paused && memory.m_globals->m_frame_time > 0.f )
	//	globals.m_local->tick_base( )++;
}

void prediction_t::restore_entity_to_predicted_frame( int frame )
{
	static auto fn = reinterpret_cast< void( __stdcall* )( int, int ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( "55 8B EC 8B 4D ? 56 E8 ? ? ? ? 8B 75" ) );
	fn( 0, frame );
}
