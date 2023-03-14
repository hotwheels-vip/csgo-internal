#include "prediction.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

void n_prediction::impl_t::update( )
{
	auto nci = g_interfaces.m_engine_client->get_net_channel_info( );
	if ( nci ) {
		g_ctx.m_max_allocations = g_math.time_to_ticks( g_convars[ HASH_BT( "sv_maxunlag" ) ]->get_float( ) );

		// add 200 ms if extended is on
		if ( GET_VARIABLE( g_variables.m_backtrack_extend, bool ) )
			// cannot get the right value for this. do not know whats happening.
			// tried unlg - flow outgoing, didnt work
			// tried unlag - outoing and incoming, weird glitches
			// if anyone can figure this out, please and thank you
			g_ctx.m_max_allocations += g_math.time_to_ticks( nci->get_latency( FLOW_INCOMING ) + nci->get_latency( FLOW_OUTGOING ) );
	} else
		g_ctx.m_max_allocations = 0;

	// handle backup data
	g_prediction.backup_data.m_flags         = g_ctx.m_local->get_flags( );
	g_prediction.backup_data.m_move_type     = g_ctx.m_local->get_move_type( );
	g_prediction.backup_data.m_velocity      = g_ctx.m_local->get_velocity( );
	g_prediction.backup_data.m_fall_velocity = g_ctx.m_local->get_fall_velocity( );
	g_prediction.backup_data.m_origin        = g_ctx.m_local->get_abs_origin( );
	g_prediction.backup_data.m_view_angles   = g_ctx.m_cmd->m_view_point;

	if ( const bool valid = g_interfaces.m_client_state->m_delta_tick > 0; valid )
		g_interfaces.m_prediction->update( g_interfaces.m_client_state->m_delta_tick, valid, g_interfaces.m_client_state->m_last_command_ack,
		                                   g_interfaces.m_client_state->m_last_outgoing_command + g_interfaces.m_client_state->m_choked_commands );
}

void n_prediction::impl_t::begin( c_base_entity* local, c_user_cmd* cmd )
{
	if ( !g_interfaces.m_move_helper )
		return;

	*local->get_current_command( ) = cmd;
	local->get_last_command( )     = *cmd;

	if ( !m_prediction_random_seed && !m_prediction_player ) {
		m_prediction_random_seed = *reinterpret_cast< unsigned int** >( g_modules[ CLIENT_DLL ].find_pattern( "A3 ? ? ? ? 66 ? ? 86" ) + 0x1 );

		m_prediction_player = *reinterpret_cast< c_base_entity*** >( g_modules[ CLIENT_DLL ].find_pattern( "89 35 ? ? ? ? F3 0F 10 48 20" ) + 0x2 );
	}

	*m_prediction_random_seed = cmd->m_random_seed & 0x7FFFFFFF;
	*m_prediction_player      = local;

	m_old_current_time = g_interfaces.m_global_vars_base->m_current_time;
	m_old_frame_time   = g_interfaces.m_global_vars_base->m_frame_time;
	m_old_tick_count   = g_interfaces.m_global_vars_base->m_tick_count;

	const bool old_is_first_prediction = g_interfaces.m_prediction->m_is_first_time_predicted;
	const bool old_in_prediction       = g_interfaces.m_prediction->m_in_prediction;

	g_interfaces.m_global_vars_base->m_current_time =
		static_cast< float >( local->get_tick_base( ) ) * g_interfaces.m_global_vars_base->m_interval_per_tick;

	g_interfaces.m_global_vars_base->m_frame_time =
		g_interfaces.m_prediction->m_engine_paused ? 0.f : g_interfaces.m_global_vars_base->m_interval_per_tick;

	g_interfaces.m_global_vars_base->m_tick_count = local->get_tick_base( );

	g_interfaces.m_prediction->m_is_first_time_predicted = false;
	g_interfaces.m_prediction->m_in_prediction           = true;

	cmd->m_buttons |= local->get_button_forced( );
	cmd->m_buttons &= ~( local->get_button_disabled( ) );

	g_interfaces.m_game_movement->start_track_prediction_errors( local );

	g_prediction.update_button_state( local, cmd );

	g_interfaces.m_prediction->check_moving_ground( local, g_interfaces.m_global_vars_base->m_frame_time );

	g_interfaces.m_prediction->set_local_view_angles( cmd->m_view_point );

	if ( local->physics_run_think( 0 ) )
		local->pre_think( );

	if ( int* next_think_tick = local->get_next_think_tick( ); *next_think_tick > 0 && *next_think_tick <= get_corrected_tick_base( local, cmd ) ) {
		*next_think_tick = -1;

		local->set_next_think( 0 );

		local->think( );
	}

	g_interfaces.m_move_helper->set_host( local );

	c_move_data m_move_data{ };

	g_interfaces.m_prediction->setup_move( local, cmd, g_interfaces.m_move_helper, &m_move_data );
	g_interfaces.m_game_movement->process_movement( local, &m_move_data );
	g_interfaces.m_prediction->finish_move( local, cmd, &m_move_data );

	g_interfaces.m_move_helper->process_impacts( );

	local->post_think( );

	g_interfaces.m_prediction->m_in_prediction           = old_in_prediction;
	g_interfaces.m_prediction->m_is_first_time_predicted = old_is_first_prediction;
}

void n_prediction::impl_t::end( c_base_entity* local ) const
{
	if ( !g_interfaces.m_move_helper )
		return;

	g_interfaces.m_game_movement->finish_track_prediction_errors( local );

	g_interfaces.m_move_helper->set_host( nullptr );

	g_interfaces.m_global_vars_base->m_current_time = m_old_current_time;
	g_interfaces.m_global_vars_base->m_frame_time   = m_old_frame_time;
	g_interfaces.m_global_vars_base->m_tick_count   = m_old_tick_count;

	*local->get_current_command( ) = nullptr;

	*m_prediction_random_seed = -1;

	*m_prediction_player = nullptr;

	g_interfaces.m_game_movement->reset( );
}

int n_prediction::impl_t::get_corrected_tick_base( c_base_entity* local, c_user_cmd* cmd )
{
	static int corrected_tick_base = 0;

	if ( static c_user_cmd* last_cmd = nullptr; cmd ) {
		if ( !last_cmd || last_cmd->m_has_been_predicted )
			corrected_tick_base = local->get_tick_base( );
		else
			corrected_tick_base++;

		last_cmd = cmd;
	}

	return corrected_tick_base;
}

void n_prediction::impl_t::update_button_state( c_base_entity* local, c_user_cmd* cmd )
{
	const int buttons         = cmd->m_buttons;
	const int local_buttons   = *local->get_buttons( );
	const int buttons_changed = buttons ^ local_buttons;

	local->get_button_last( )     = local_buttons;
	*local->get_buttons( )        = buttons;
	local->get_button_pressed( )  = buttons_changed & buttons;
	local->get_button_released( ) = buttons_changed & ( ~buttons );
};

void n_prediction::impl_t::restore_entity_to_predicted_frame( int frame )
{
	// sanity
	if ( g_ctx.m_local->get_observer_mode( ) > 0 )
		return;

	static auto cl_predict = g_convars[ HASH_BT( "cl_predict" ) ];
	if ( !cl_predict->get_int( ) )
		return;

	if ( !g_ctx.m_local->is_predictable( ) )
		return;

	g_ctx.m_local->restore_data( "RestoreEntityToPredictedFrame", frame, 2 /* PC_EVERYTHING */ );
	g_ctx.m_local->on_post_restore_data( );
}
