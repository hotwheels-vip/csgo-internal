#include "prediction.h"

#include "../../globals/ctx/ctx.h"
#include "../../globals/hooks/cl_move/cl_move.h"
#include "../../globals/interfaces/interfaces.h"

void prediction::impl::store_backup( )
{
	g_prediction.backup_vars.flags         = g_ctx.local->flags( );
	g_prediction.backup_vars.velocity      = g_ctx.local->velocity( );
	g_prediction.backup_vars.fall_velocity = g_ctx.local->fall_velocity( );
	g_prediction.backup_vars.move_type     = g_ctx.local->move_type( );
	g_prediction.backup_vars.view_angles   = g_ctx.cmd->view_angles;
}

void prediction::impl::update( )
{
	// delta_tick is the delta between server tick and current frame time, for example if u're in a 64 tick server and u have 63 fps u have 1
	// delta_tick, that means prediction will run update function whenever ur fps is lower then tickrate, making it less likely to have errors/bugs
	// with low fps
	if ( g_interfaces.client_state->delta_tick > 0 )
		g_interfaces.prediction->update( g_interfaces.client_state->delta_tick, g_interfaces.client_state->delta_tick > 0,
		                                 g_interfaces.client_state->last_command_ack,
		                                 g_interfaces.client_state->last_outgoing_command + g_interfaces.client_state->choked_commands );
}

void prediction::impl::start( sdk::c_base_player* player )
{
	// TODO: fix prediction noises, alot of stuff in this prediction is either in the wrong place or not needed.

	g_prediction.in_prediction = true;

	player->current_command( ) = g_ctx.cmd;

	sdk::c_base_player::set_prediction_random_seed( g_ctx.cmd );
	sdk::c_base_player::set_prediction_player( player );

	reset_vars.tick_base    = get_tick_base( player );
	reset_vars.current_time = g_interfaces.globals->current_time;
	reset_vars.frame_time   = g_interfaces.globals->frame_time;

	m_backup_is_first_time_predicted = g_interfaces.prediction->m_is_first_time_predicted;
	m_backup_in_prediction           = g_interfaces.prediction->m_in_prediction;

	g_interfaces.prediction->m_in_prediction           = true;
	g_interfaces.prediction->m_is_first_time_predicted = false;

	g_interfaces.globals->current_time = sdk::ticks_to_time( player->tick_base( ) );
	g_interfaces.globals->frame_time   = g_interfaces.prediction->m_engine_paused ? 0 : g_interfaces.globals->interval_per_tick;

	g_interfaces.game_movement->start_track_prediction_errors( player );

	if ( g_ctx.cmd->weapon_select ) {
		auto weapon = g_interfaces.entity_list->get_client_entity< sdk::c_base_combat_weapon* >( g_ctx.cmd->weapon_select );
		if ( weapon ) {
			auto weapon_info = weapon->get_weapon_data( );
			if ( weapon_info )
				player->select_item( weapon_info->weapon_name, g_ctx.cmd->weapon_subtype );
		}
	}

	g_interfaces.prediction->set_local_view_angles( g_ctx.cmd->view_angles );

	player->pre_think( );

	g_interfaces.move_helper->set_host( player );

	g_interfaces.prediction->setup_move( player, g_ctx.cmd, g_interfaces.move_helper, &reset_vars.move_data );

	g_interfaces.game_movement->process_movement( player, &reset_vars.move_data );

	g_interfaces.prediction->finish_move( player, g_ctx.cmd, &reset_vars.move_data );

	g_interfaces.move_helper->process_impacts( );

	g_ctx.running_post_think = true;
	player->post_think( );
	g_ctx.running_post_think = false;

	player->tick_base( )                               = reset_vars.tick_base;
	g_interfaces.prediction->m_is_first_time_predicted = m_backup_is_first_time_predicted;
	g_interfaces.prediction->m_in_prediction           = m_backup_in_prediction;
	// player->adjust_player_timebase( simulation_ticks );
}

void prediction::impl::end( sdk::c_base_player* player )
{
	if ( !player )
		return; // kys

	g_interfaces.game_movement->finish_track_prediction_errors( player );

	g_interfaces.move_helper->set_host( nullptr );

	g_interfaces.globals->current_time = reset_vars.current_time;
	g_interfaces.globals->frame_time   = reset_vars.frame_time;

	player->current_command( ) = nullptr;

	sdk::c_base_player::set_prediction_random_seed( nullptr );
	sdk::c_base_player::set_prediction_player( nullptr );

	g_interfaces.game_movement->reset( );

	g_prediction.in_prediction = false;
}

void prediction::impl::reset( )
{
	g_interfaces.prediction->restore_entity_to_predicted_frame( g_interfaces.prediction->predicted_commands( ) - 1 );
	g_interfaces.prediction->previous_start_frame( ) = -1;
	g_interfaces.prediction->predicted_commands( )   = 0;
}

int prediction::impl::get_tick_base( sdk::c_base_player* player )
{
	static int tick = 0;

	if ( g_ctx.cmd ) {
		static sdk::c_user_cmd* last_command{ };

		if ( !last_command || last_command->has_been_predicted )
			tick = player->tick_base( );
		else
			tick++;

		last_command = g_ctx.cmd;
	}

	return tick;
}
