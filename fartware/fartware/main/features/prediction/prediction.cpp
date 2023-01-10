#include "prediction.h"
#include "../../globals.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../../utilities/input/input.h"
#include "../movement/movement.h"

void prediction_t::begin( )
{
	*globals.m_local->current_command( ) = globals.m_cmd;
	globals.m_local->last_command( )     = *globals.m_cmd;

	if ( !this->m_prediction_random_seed || !this->m_prediction_player ) {
		this->m_prediction_random_seed = *reinterpret_cast< unsigned int** >(
			memory.m_modules[ e_module_names::client ].find_pattern( ( "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04" ) ) + 0x2 );
		this->m_prediction_player = *reinterpret_cast< c_base_entity*** >(
			memory.m_modules[ e_module_names::client ].find_pattern( ( "89 35 ? ? ? ? F3 0F 10 48 20" ) ) + 0x2 );
	}

	*this->m_prediction_random_seed = globals.m_cmd->m_random_seed;
	*this->m_prediction_player      = globals.m_local;

	this->m_old_current_time = memory.m_globals->m_current_time;
	this->m_old_frame_time   = memory.m_globals->m_frame_time;
	this->m_old_tick_count   = memory.m_globals->m_tick_count;

	const int old_tick_base = globals.m_local->tick_base( );

	const bool old_is_first_prediction = interfaces.m_prediction->m_is_first_time_predicted;
	const bool old_in_prediction       = interfaces.m_prediction->m_in_prediction;

	memory.m_globals->m_current_time = memory.m_globals->m_interval_per_tick * static_cast< float >( this->get_tick_base( ) );
	memory.m_globals->m_frame_time   = interfaces.m_prediction->m_engine_paused ? 0.f : memory.m_globals->m_interval_per_tick;
	memory.m_globals->m_tick_count   = this->get_tick_base( );

	interfaces.m_prediction->m_is_first_time_predicted = false;
	interfaces.m_prediction->m_in_prediction           = true;

	globals.m_cmd->m_buttons |= globals.m_local->button_forced( );
	globals.m_cmd->m_buttons &= ~( globals.m_local->button_disabled( ) );

	interfaces.m_game_movement->start_track_prediction_errors( globals.m_local );

	if ( globals.m_cmd->m_weapon_select ) {
		static auto active_weapon = reinterpret_cast< c_base_entity* >(
			interfaces.m_client_entity_list->get_client_entity_from_handle( globals.m_local->active_weapon_handle( ) ) );
		if ( active_weapon ) {
			static auto weapon_data = memory.m_weapon_system->get_weapon_data( active_weapon->item_definition_index( ) );
			if ( weapon_data )
				globals.m_local->select_item( weapon_data->m_weapon_name, globals.m_cmd->m_weapon_sub_type );
		}
	}

	const int buttons         = globals.m_cmd->m_buttons;
	const int local_buttons   = *globals.m_local->buttons( );
	const int buttons_changed = buttons ^ local_buttons;

	globals.m_local->button_last( )     = local_buttons;
	*globals.m_local->buttons( )        = buttons;
	globals.m_local->button_pressed( )  = buttons_changed & buttons;
	globals.m_local->button_released( ) = buttons_changed & ( ~buttons );

	interfaces.m_prediction->check_moving_ground( globals.m_local, memory.m_globals->m_frame_time );

	interfaces.m_prediction->set_local_view_angles( globals.m_cmd->m_view_point );

	if ( globals.m_local->physics_run_think( 0 ) )
		globals.m_local->pre_think( );

	if ( int* next_think_tick = globals.m_local->next_think_tick( ); *next_think_tick > 0 && *next_think_tick <= this->get_tick_base( ) ) {
		*next_think_tick = -1;

		globals.m_local->set_next_think( 0 );

		globals.m_local->think( );
	}

	memory.m_move_helper->set_host( globals.m_local );

	interfaces.m_prediction->setup_move( globals.m_local, globals.m_cmd, memory.m_move_helper, &this->m_move_data );

	interfaces.m_game_movement->process_movement( globals.m_local, &this->m_move_data );

	interfaces.m_prediction->finish_move( globals.m_local, globals.m_cmd, &this->m_move_data );
	memory.m_move_helper->process_impacts( );

	globals.m_local->post_think( );

	globals.m_local->tick_base( ) = old_tick_base;

	interfaces.m_prediction->m_in_prediction           = old_in_prediction;
	interfaces.m_prediction->m_is_first_time_predicted = old_is_first_prediction;
}

void prediction_t::end( ) const
{
	interfaces.m_game_movement->finish_track_prediction_errors( globals.m_local );

	memory.m_move_helper->set_host( nullptr );

	memory.m_globals->m_current_time = this->m_old_current_time;
	memory.m_globals->m_frame_time   = this->m_old_frame_time;
	memory.m_globals->m_tick_count   = this->m_old_tick_count;

	*globals.m_local->current_command( ) = nullptr;

	*this->m_prediction_random_seed = -1;

	*this->m_prediction_player = nullptr;

	interfaces.m_game_movement->reset( );
}

int prediction_t::get_tick_base( )
{
	static int tick = 0;

	static c_user_cmd* last_cmd = nullptr;

	if ( last_cmd == nullptr || last_cmd->m_has_been_predicted )
		tick = globals.m_local->tick_base( );
	else
		tick++;

	last_cmd = globals.m_cmd;

	return tick;
}
