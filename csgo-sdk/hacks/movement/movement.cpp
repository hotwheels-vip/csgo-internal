#include "movement.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../prediction/prediction.h"

void n_movement::impl_t::on_create_move_pre( )
{
	if ( GET_VARIABLE( g_variables.m_no_crouch_cooldown, bool ) )
		g_ctx.m_cmd->m_buttons |= e_command_buttons::in_bullrush;

	const auto move_type = g_ctx.m_local->get_move_type( );
	if ( move_type == e_move_types::move_type_ladder || move_type == e_move_types::move_type_noclip || move_type == e_move_types::move_type_fly ||
	     move_type == e_move_types::move_type_observer )
		return;

	if ( GET_VARIABLE( g_variables.m_bunny_hop, bool ) &&
	     !( GET_VARIABLE( g_variables.m_jump_bug, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_jump_bug_key, key_bind_t ) ) ) )
		this->bunny_hop( );
}

void n_movement::impl_t::bunny_hop( )
{
	if ( !( g_ctx.m_local->get_flags( ) & e_flags::fl_onground ) )
		g_ctx.m_cmd->m_buttons &= ~e_command_buttons::in_jump;
}

void n_movement::impl_t::on_create_move_post( )
{
	const float target_ps_velocity =
		-g_convars[ HASH_BT( "sv_gravity" ) ]->get_float( ) * 0.5f * g_interfaces.m_global_vars_base->m_interval_per_tick;

	if ( GET_VARIABLE( g_variables.m_edge_jump, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_edge_jump_key, key_bind_t ) ) )
		this->edge_jump( );

	// run this before movetype check so we can auto ladder bug up stairs(temporary)
	if ( GET_VARIABLE( g_variables.m_jump_bug, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_jump_bug_key, key_bind_t ) ) )
		this->jump_bug( );

	const auto move_type = g_prediction.backup_data.m_move_type;
	if ( move_type == e_move_types::move_type_ladder || move_type == e_move_types::move_type_noclip || move_type == e_move_types::move_type_fly ||
	     move_type == e_move_types::move_type_observer )
		return;

	if ( GET_VARIABLE( g_variables.m_long_jump, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_long_jump_key, key_bind_t ) ) )
		this->long_jump( );

	if ( GET_VARIABLE( g_variables.m_mini_jump, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_mini_jump_key, key_bind_t ) ) )
		this->mini_jump( );

	if ( GET_VARIABLE( g_variables.m_auto_align, bool ) && !( g_prediction.backup_data.m_flags & e_flags::fl_onground ||
	                                                          this->m_pixelsurf_data.m_in_pixel_surf || this->m_edgebug_data.m_will_edgebug ) )
		this->auto_align( g_ctx.m_cmd );

	this->pixel_surf( target_ps_velocity );

	this->auto_duck( );

	this->movement_fix( g_prediction.backup_data.m_view_angles );

	this->pixel_surf_locking( target_ps_velocity );

	this->edge_bug( );
}

void n_movement::impl_t::edge_jump( )
{
	const auto move_type = g_prediction.backup_data.m_move_type;
	if ( move_type == e_move_types::move_type_noclip || move_type == e_move_types::move_type_fly || move_type == e_move_types::move_type_observer )
		return;

	// is on ladder and wont be on ladder on next tick
	if ( move_type == e_move_types::move_type_ladder ) {
		// no need to restore these since we only run it on ladders

		g_prediction.begin( g_ctx.m_local, g_ctx.m_cmd );
		g_prediction.end( g_ctx.m_local );
		g_prediction.begin( g_ctx.m_local, g_ctx.m_cmd );
		g_prediction.end( g_ctx.m_local );

		if ( g_ctx.m_local->get_move_type( ) != e_move_type::movetype_ladder ) {
			if ( GET_VARIABLE( g_variables.m_edge_jump_ladder, bool ) )
				g_ctx.m_cmd->m_buttons |= e_command_buttons::in_jump;
		} else // remove jump flag while in ladder
			g_ctx.m_cmd->m_buttons &= ~e_command_buttons::in_jump;

		return;
	}

	if ( ( g_prediction.backup_data.m_flags & e_flags::fl_onground ) && !( g_ctx.m_local->get_flags( ) & e_flags::fl_onground ) )
		g_ctx.m_cmd->m_buttons |= e_command_buttons::in_jump;
}

void n_movement::impl_t::edge_bug( )
{
	if ( !( GET_VARIABLE( g_variables.m_edge_bug, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_edge_bug_key, key_bind_t ) ) ) ||
	     g_movement.m_pixelsurf_data.m_in_pixel_surf ) {
		m_edgebug_data.reset( );
		return;
	}

	const float yaw_delta =
		std::clamp( g_prediction.backup_data.m_view_angles.m_y - g_ctx.m_last_tick_yaw,
	                -( 180.f / GET_VARIABLE( g_variables.m_edge_bug_ticks, int ) ), 180.f / GET_VARIABLE( g_variables.m_edge_bug_ticks, int ) );

	const float original_forward_move = g_ctx.m_cmd->m_forward_move;
	const float original_side_move    = g_ctx.m_cmd->m_side_move;
	const auto original_view_point    = g_ctx.m_cmd->m_view_point;

	const auto loop_through_ticks = [ & ]( const bool ducked, const bool strafe = false ) {
		if ( m_edgebug_data.m_will_edgebug )
			return;

		g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );

		g_movement.m_edgebug_data.m_starting_yaw = original_view_point.m_y;

		for ( int i = 0; i <= GET_VARIABLE( g_variables.m_edge_bug_ticks, int ); i++ ) {
			c_user_cmd* simulated_cmd = new c_user_cmd( *g_ctx.m_cmd );

			simulated_cmd->m_buttons |= e_command_buttons::in_bullrush;

			if ( ducked ) {
				simulated_cmd->m_buttons |= e_command_buttons::in_duck;
				g_ctx.m_local->get_flags( ) |= e_flags::fl_ducking;
			} else {
				simulated_cmd->m_buttons &= ~e_command_buttons::in_duck;
				g_ctx.m_local->get_flags( ) &= ~e_flags::fl_ducking;
			}

			if ( !strafe ) {
				m_edgebug_data.m_strafing = false;

				simulated_cmd->m_forward_move = 0;
				simulated_cmd->m_side_move    = 0;

				// p sure these are not needed, but just making sure
				simulated_cmd->m_buttons &= ~( e_command_buttons::in_jump | e_command_buttons::in_forward | e_command_buttons::in_back |
				                               e_command_buttons::in_moveleft | e_command_buttons::in_moveright );
			} else // strafed
			{
				m_edgebug_data.m_strafing = true;

				simulated_cmd->m_forward_move = original_forward_move;
				simulated_cmd->m_side_move    = original_side_move;

				simulated_cmd->m_view_point.m_y = g_math.normalize_angle( original_view_point.m_y + ( yaw_delta * i ) );
			}

			g_prediction.begin( g_ctx.m_local, simulated_cmd );
			g_prediction.end( g_ctx.m_local );

			if ( g_prediction.backup_data.m_flags & e_flags::fl_onground || round( g_prediction.backup_data.m_velocity.m_z ) >= 0 ||
			     g_ctx.m_local->get_flags( ) & fl_onground ) {
				m_edgebug_data.m_will_edgebug = false;
				break;
			}

			if ( !m_edgebug_data.m_will_edgebug )
				this->detect_edgebug( simulated_cmd );

			if ( m_edgebug_data.m_will_edgebug ) {
				m_edgebug_data.m_saved_mousedx = abs( simulated_cmd->m_mouse_delta_x );
				m_edgebug_data.m_ticks_to_stop = i + 1;
				m_edgebug_data.m_last_tick     = g_interfaces.m_global_vars_base->m_tick_count;

				if ( ducked )
					m_edgebug_data.m_edgebug_method = edgebug_type_t::eb_ducking;
				else
					m_edgebug_data.m_edgebug_method = edgebug_type_t::eb_standing;

				if ( strafe ) {
					m_edgebug_data.m_yaw_delta = yaw_delta;

					m_edgebug_data.m_forward_move = original_forward_move;
					m_edgebug_data.m_side_move    = original_side_move;
				} else {
					m_edgebug_data.m_forward_move = m_edgebug_data.m_side_move = 0;
				}
				break;
			}

			if ( m_edgebug_data.m_will_fail ) {
				m_edgebug_data.m_will_fail = false;
				break;
			}

			delete simulated_cmd;
		}

		g_prediction.begin( g_ctx.m_local, g_ctx.m_cmd );
		g_prediction.end( g_ctx.m_local );

		g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );
	};

	// non strafed edgebugs
	loop_through_ticks( edgebug_type_t::eb_standing );
	loop_through_ticks( edgebug_type_t::eb_ducking );

	// strafed edgebugs
	if ( GET_VARIABLE( g_variables.m_advanced_detection, bool ) &&
	     yaw_delta < ( GET_VARIABLE( g_variables.m_edge_bug_strafe_delta_max, float ) / 1000.f ) ) {
		loop_through_ticks( edgebug_type_t::eb_standing, true );
		loop_through_ticks( edgebug_type_t::eb_ducking, true );
	}

	if ( m_edgebug_data.m_will_edgebug ) {
		g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );

		if ( g_interfaces.m_global_vars_base->m_tick_count < m_edgebug_data.m_ticks_to_stop + m_edgebug_data.m_last_tick + 1 ) {
			g_ctx.m_cmd->m_buttons &= ~( e_command_buttons::in_jump | e_command_buttons::in_forward | e_command_buttons::in_back |
			                             e_command_buttons::in_moveleft | e_command_buttons::in_moveright );

			if ( m_edgebug_data.m_strafing ) {
				g_ctx.m_cmd->m_side_move    = m_edgebug_data.m_side_move;
				g_ctx.m_cmd->m_forward_move = m_edgebug_data.m_forward_move;

				const float final_yaw = g_math.normalize_angle(
					m_edgebug_data.m_starting_yaw +
					( m_edgebug_data.m_yaw_delta * ( g_interfaces.m_global_vars_base->m_tick_count - m_edgebug_data.m_last_tick ) ) );

				g_ctx.m_cmd->m_view_point.m_y = final_yaw;

			} else {
				g_ctx.m_cmd->m_side_move    = 0.f;
				g_ctx.m_cmd->m_forward_move = 0.f;
			}
			if ( m_edgebug_data.m_edgebug_method == edgebug_type_t::eb_ducking ) {
				g_ctx.m_cmd->m_buttons |= in_duck;
			} else
				g_ctx.m_cmd->m_buttons &= ~in_duck;
		} else
			m_edgebug_data.reset( );
	}
}

void n_movement::impl_t::long_jump( )
{
	static int saved_tick = 0;

	if ( g_ctx.m_local->get_flags( ) & e_flags::fl_onground )
		saved_tick = g_interfaces.m_global_vars_base->m_tick_count;

	if ( !( g_interfaces.m_global_vars_base->m_tick_count - saved_tick > 2 ) && !( g_ctx.m_local->get_flags( ) & e_flags::fl_onground ) )
		g_ctx.m_cmd->m_buttons |= e_command_buttons::in_duck;
}

void n_movement::impl_t::mini_jump( )
{
	static bool should_duck = false;

	if ( g_prediction.backup_data.m_flags && g_ctx.m_local->get_flags( ) & e_flags::fl_onground )
		should_duck = false;

	if ( g_prediction.backup_data.m_flags & e_flags::fl_onground && !( g_ctx.m_local->get_flags( ) & e_flags::fl_onground ) ) {
		g_ctx.m_cmd->m_buttons |= e_command_buttons::in_jump;
		g_ctx.m_cmd->m_buttons |= e_command_buttons::in_duck;
		if ( GET_VARIABLE( g_variables.m_mini_jump_hold_duck, bool ) )
			should_duck = true;
	}

	if ( should_duck )
		g_ctx.m_cmd->m_buttons |= e_command_buttons::in_duck;
}

void n_movement::impl_t::auto_duck( )
{
	if ( !GET_VARIABLE( g_variables.m_auto_duck, bool ) || g_prediction.backup_data.m_flags & e_flags::fl_onground ||
	     g_movement.m_edgebug_data.m_will_edgebug || g_movement.m_pixelsurf_data.m_in_pixel_surf ||
	     ( GET_VARIABLE( g_variables.m_jump_bug, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_jump_bug_key, key_bind_t ) ) ) ||
	     ( GET_VARIABLE( g_variables.m_edge_jump, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_edge_jump_key, key_bind_t ) ) ) ) {
		g_movement.m_autoduck_data.reset( );

		return;
	}

	g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );

	for ( int i = 0; i < 2; i++ ) {
		if ( g_ctx.m_local->get_flags( ) & e_flags::fl_onground )
			break;

		c_user_cmd* simulated_cmd = new c_user_cmd( *g_ctx.m_cmd );

		simulated_cmd->m_buttons |= e_command_buttons::in_bullrush;
		simulated_cmd->m_buttons |= e_command_buttons::in_duck;

		g_prediction.begin( g_ctx.m_local, simulated_cmd );
		g_prediction.end( g_ctx.m_local );

		if ( g_ctx.m_local->get_flags( ) & e_flags::fl_onground ) {
			g_movement.m_autoduck_data.m_did_land_ducking = true;
			g_movement.m_autoduck_data.m_ducking_vert     = g_ctx.m_local->get_abs_origin( ).m_z;
			break;
		}
		delete simulated_cmd;
	}

	g_prediction.begin( g_ctx.m_local, g_ctx.m_cmd );
	g_prediction.end( g_ctx.m_local );

	g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );

	if ( !g_movement.m_autoduck_data.m_did_land_ducking )
		return;

	for ( int i = 0; i < 2; i++ ) {
		if ( g_ctx.m_local->get_flags( ) & e_flags::fl_onground )
			break;

		c_user_cmd* simulated_cmd = new c_user_cmd( *g_ctx.m_cmd );

		simulated_cmd->m_buttons &= ~e_command_buttons::in_duck;

		g_prediction.begin( g_ctx.m_local, simulated_cmd );
		g_prediction.end( g_ctx.m_local );

		if ( g_ctx.m_local->get_flags( ) & e_flags::fl_onground ) {
			g_movement.m_autoduck_data.m_did_land_standing = true;
			g_movement.m_autoduck_data.m_standing_vert     = g_ctx.m_local->get_abs_origin( ).m_z;
			break;
		}
		delete simulated_cmd;
	}

	g_prediction.begin( g_ctx.m_local, g_ctx.m_cmd );
	g_prediction.end( g_ctx.m_local );

	g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );

	if ( g_movement.m_autoduck_data.m_did_land_ducking && g_movement.m_autoduck_data.m_did_land_standing ) {
		if ( g_movement.m_autoduck_data.m_ducking_vert > g_movement.m_autoduck_data.m_standing_vert )
			g_ctx.m_cmd->m_buttons |= e_command_buttons::in_duck;
	} else if ( g_movement.m_autoduck_data.m_did_land_ducking && !g_movement.m_autoduck_data.m_did_land_standing )
		g_ctx.m_cmd->m_buttons |= e_command_buttons::in_duck;
}

void n_movement::impl_t::pixel_surf_locking( float target_ps_velocity )
{
	if ( g_movement.m_pixelsurf_data.m_in_pixel_surf ) {
		g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );

		if ( !g_movement.m_pixelsurf_data.m_predicted_succesful ) {
			if ( g_movement.m_pixelsurf_data.m_prediction_ticks < g_ctx.m_cmd->m_tick_count )
				g_movement.m_pixelsurf_data.m_predicted_succesful = true;

			g_ctx.m_cmd->m_buttons      = g_movement.m_pixelsurf_data.m_simulated_cmd->m_buttons;
			g_ctx.m_cmd->m_side_move    = g_movement.m_pixelsurf_data.m_simulated_cmd->m_side_move;
			g_ctx.m_cmd->m_forward_move = g_movement.m_pixelsurf_data.m_simulated_cmd->m_forward_move;

			const c_vector movement = { g_ctx.m_cmd->m_forward_move, g_ctx.m_cmd->m_side_move, 0 };
			c_angle movement_angle{ };
			g_math.vector_angles( movement, movement_angle );

			const float rotation =
				deg2rad( g_ctx.m_cmd->m_view_point.m_y - g_movement.m_pixelsurf_data.m_simulated_cmd->m_view_point.m_y + movement_angle.m_y );

			g_ctx.m_cmd->m_forward_move = std::cosf( rotation ) * movement.length_2d( );
			g_ctx.m_cmd->m_side_move    = std::sinf( rotation ) * movement.length_2d( );
		}

		if ( g_movement.m_pixelsurf_data.m_should_duck )
			g_ctx.m_cmd->m_buttons |= e_command_buttons::in_duck;
		else
			g_ctx.m_cmd->m_buttons &= ~e_command_buttons::in_duck;

		if ( g_movement.m_pixelsurf_data.m_predicted_succesful && !( g_ctx.m_local->get_velocity( ).m_z == target_ps_velocity ) ) {
			g_movement.m_pixelsurf_data.m_in_pixel_surf       = false;
			g_movement.m_pixelsurf_data.m_predicted_succesful = false;
			return;
		}
		return;
	}
}

void n_movement::impl_t::pixel_surf( float target_ps_velocity )
{
	if ( !( GET_VARIABLE( g_variables.m_pixel_surf, bool ) && g_input.check_input( &GET_VARIABLE( g_variables.m_pixel_surf_key, key_bind_t ) ) ) ) {
		g_movement.m_pixelsurf_data.reset( );
		return;
	}

	if ( g_movement.m_pixelsurf_data.m_in_pixel_surf || g_ctx.m_local->get_flags( ) & e_flags::fl_onground ||
	     g_ctx.m_local->get_velocity( ).m_z >= 0.f )
		return;

	if ( !g_movement.m_pixelsurf_data.m_predicted_succesful ) {
		g_movement.m_pixelsurf_data.m_should_duck = false;
		for ( int prediction_type = 0; prediction_type < 2; prediction_type++ ) {
			if ( g_movement.m_pixelsurf_data.m_in_pixel_surf )
				break;

			g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );

			int backup_flags         = g_ctx.m_local->get_flags( );
			c_vector backup_velocity = g_ctx.m_local->get_velocity( );

			for ( int i = 0; i < 12; i++ ) {
				c_user_cmd* simulated_cmd = new c_user_cmd( *g_ctx.m_cmd );

				simulated_cmd->m_buttons |= e_command_buttons::in_bullrush;

				if ( prediction_type == 0 )
					simulated_cmd->m_buttons |= e_command_buttons::in_duck;
				else
					simulated_cmd->m_buttons &= ~e_command_buttons::in_duck;

				// auto align on pixel surf has to automatically be on.
				auto_align( simulated_cmd );

				g_prediction.begin( g_ctx.m_local, simulated_cmd );
				g_prediction.end( g_ctx.m_local );

				if ( backup_flags & e_flags::fl_onground )
					break;

				if ( g_ctx.m_local->get_velocity( ).m_z == target_ps_velocity && backup_velocity.m_z == target_ps_velocity ) {
					g_movement.m_pixelsurf_data.m_prediction_ticks = g_ctx.m_cmd->m_tick_count + i;
					g_movement.m_pixelsurf_data.m_simulated_cmd    = simulated_cmd;
					g_movement.m_pixelsurf_data.m_should_duck      = !( prediction_type == 1 );
					g_movement.m_pixelsurf_data.m_in_pixel_surf    = true;
					break;
				}

				backup_velocity = g_ctx.m_local->get_velocity( );
				backup_flags    = g_ctx.m_local->get_flags( );

				delete simulated_cmd;
			}

			g_prediction.begin( g_ctx.m_local, g_ctx.m_cmd );
			g_prediction.end( g_ctx.m_local );

			g_prediction.restore_entity_to_predicted_frame( g_interfaces.m_prediction->m_commands_predicted - 1 );
		}
	}
}

void n_movement::impl_t::jump_bug( )
{
	[[unlikely]] if ( !( g_ctx.m_cmd->m_buttons & e_command_buttons::in_jump ) )
	{
		static bool ducked = false;

		if ( g_ctx.m_local->get_flags( ) & e_flags::fl_onground && !( g_prediction.backup_data.m_flags & e_flags::fl_onground ) && !ducked ) {
			g_ctx.m_cmd->m_buttons |= e_command_buttons::in_duck;
			ducked = true;
		} else
			ducked = false;

		if ( g_prediction.backup_data.m_flags & e_flags::fl_onground && ducked )
			ducked = false;
	}
	else
	{
		if ( g_ctx.m_local->get_flags( ) & e_flags::fl_onground && !( g_prediction.backup_data.m_flags & e_flags::fl_onground ) )
			g_ctx.m_cmd->m_buttons |= e_command_buttons::in_duck;

		if ( g_ctx.m_local->get_flags( ) & e_flags::fl_onground )
			g_ctx.m_cmd->m_buttons &= ~e_command_buttons::in_jump;

		if ( !( g_ctx.m_local->get_flags( ) & fl_onground ) && g_prediction.backup_data.m_flags & fl_onground )
			g_ctx.m_cmd->m_buttons &= ~e_command_buttons::in_duck;
	}
}

void n_movement::impl_t::rotate_movement( c_user_cmd* cmd, const c_angle& ang )
{
	c_vector vec_move = c_vector( cmd->m_forward_move, cmd->m_side_move, 0.f );

	const float speed = vec_move.length_2d( );

	const float rotation = deg2rad( cmd->m_view_point.m_y - ang.m_y );

	cmd->m_forward_move = std::cosf( rotation ) * speed;
	cmd->m_side_move    = std::sinf( rotation ) * speed;
}

void n_movement::impl_t::auto_align( c_user_cmd* cmd )
{
	const c_vector origin   = g_ctx.m_local->get_origin( );
	const c_vector velocity = g_ctx.m_local->get_velocity( );

	constexpr static float distance_till_adjust = 0.03125f;
	constexpr static float error_margin         = 0.01f;

	static const float max_fw_move = g_convars[ HASH_BT( "cl_forwardspeed" ) ]->get_float( );
	static const float max_sw_move = g_convars[ HASH_BT( "cl_sidespeed" ) ]->get_float( );

	auto get_colliding_wall = [ & ]( trace_t& out_trace ) -> bool {
		float fw_move = cmd->m_forward_move / max_fw_move;
		float sw_move = cmd->m_side_move / max_sw_move;

		c_vector va_forward = cmd->m_view_point.forward( ).normalize( ).to_vector( );
		c_vector va_right   = cmd->m_view_point.right( ).normalize( ).to_vector( );

		c_vector wish_dir = { va_forward.m_x * fw_move + va_right.m_x * sw_move, va_forward.m_y * fw_move + va_right.m_y * sw_move, 0.f };

		c_vector direct_dir = { roundf( wish_dir.m_x ), roundf( wish_dir.m_y ), 0.f };

		float trace_additive = ( distance_till_adjust + error_margin ) + g_ctx.m_local->get_collideable( )->obb_maxs( ).m_x;

		trace_t trace{ };
		c_trace_filter filter( g_ctx.m_local );

		c_vector trace_dir = g_ctx.m_local->get_abs_origin( ) + c_vector( trace_additive * direct_dir.m_x, trace_additive * direct_dir.m_y, 0.f );

		ray_t ray( g_ctx.m_local->get_abs_origin( ), trace_dir );
		g_interfaces.m_engine_trace->trace_ray( ray, 0xFFFFFFFF /*MASK_ALL*/, &filter, &trace );

		if ( trace.did_hit( ) && !trace.m_hit_entity->is_player( ) ) {
			out_trace = trace;
			return true;
		}

		return false;
	};

	constexpr auto has_to_align = []( const c_vector& origin ) -> bool {
		constexpr float distance_to_stop = 0.00200f;

		const c_vector_2d remainder1 = c_vector_2d( 1.f - ( origin.m_x - floor( origin.m_x ) ), 1.f - ( origin.m_y - floor( origin.m_y ) ) );
		const c_vector_2d remainder2 = c_vector_2d( ( origin.m_x - floor( origin.m_x ) ), ( origin.m_y - floor( origin.m_y ) ) );

		return ( ( remainder1.m_x >= distance_to_stop && remainder1.m_x <= distance_till_adjust ) ||
		         ( remainder1.m_y >= distance_to_stop && remainder1.m_y <= distance_till_adjust ) ) ||
		       ( ( remainder2.m_x >= distance_to_stop && remainder2.m_x <= distance_till_adjust ) ||
		         ( remainder2.m_y >= distance_to_stop && remainder2.m_y <= distance_till_adjust ) );
	};

	if ( this->m_pixelsurf_data.m_predicted_succesful )
		return;

	if ( velocity.length_2d( ) == 0.f || g_ctx.m_local->get_flags( ) & e_flags::fl_onground || !has_to_align( origin ) )
		return;

	trace_t hit_trace{ };
	bool is_valid = get_colliding_wall( hit_trace );

	if ( !is_valid )
		return;

	float gain_fraction = cmd->m_buttons & e_command_buttons::in_duck ? 4.6775f : 4.5500f;
	float minimum_gain  = rad2deg( atan( gain_fraction / velocity.length_2d( ) ) ) * ( 2.f * std::numbers::pi_v< float > );

	float strafe_yaw = c_vector( hit_trace.m_plane.m_normal ).to_angle( ).flip( ).m_y;

	float yaw_delta = g_math.normalize_angle( velocity.to_angle( ).m_y - strafe_yaw );

	strafe_yaw += yaw_delta < 0.f ? -minimum_gain : minimum_gain;

	c_angle strafe_angle = c_angle( cmd->m_view_point.m_x, strafe_yaw, 0.f );

	rotate_movement( cmd, strafe_angle );
}

void n_movement::impl_t::strafe_to_yaw( c_user_cmd* cmd, c_angle& angle, const float yaw )
{
	const static auto max_side_speed = g_convars[ HASH_BT( "cl_sidespeed" ) ]->get_float( );

	angle.m_y += yaw;
	cmd->m_side_move    = 0.f;
	cmd->m_forward_move = 0.f;

	const auto degrees = rad2deg( std::atan2f( g_ctx.m_local->get_velocity( ).m_y, g_ctx.m_local->get_velocity( ).m_x ) );

	const auto delta = g_math.normalize_angle( angle.m_y - degrees );

	cmd->m_side_move = delta > 0.f ? -max_side_speed : max_side_speed;
	angle.m_y        = g_math.normalize_angle( angle.m_y - delta );
}

void n_movement::impl_t::on_frame_stage_notify( int stage )
{
	if ( !g_movement.m_edgebug_data.m_will_edgebug || !g_movement.m_edgebug_data.m_strafing || stage != e_client_frame_stage::start )
		return;

	float final_yaw{ };

	c_angle wish_angles = { g_prediction.backup_data.m_view_angles.m_x, g_movement.m_edgebug_data.m_starting_yaw,
		                    g_prediction.backup_data.m_view_angles.m_z };

	float hit_time_delta = g_math.ticks_to_time( g_movement.m_edgebug_data.m_ticks_to_stop );

	float cur_time_delta = g_interfaces.m_global_vars_base->m_current_time - g_math.ticks_to_time( g_movement.m_edgebug_data.m_last_tick );

	final_yaw = g_math.normalize_angle( g_movement.m_edgebug_data.m_yaw_delta *
	                                    ( g_movement.m_edgebug_data.m_ticks_to_stop * ( cur_time_delta / hit_time_delta ) ) );

	wish_angles.m_y += final_yaw;

	g_interfaces.m_engine_client->set_view_angles( wish_angles );
}

void n_movement::impl_t::detect_edgebug( c_user_cmd* cmd )
{
	if ( g_prediction.backup_data.m_velocity.m_z > 0 || g_utilities.is_in< int >( g_ctx.m_local->get_move_type( ), invalid_move_types ) ) {
		m_edgebug_data.m_will_edgebug = false;
		m_edgebug_data.m_will_fail    = true;
		return;
	}

	if ( round( g_ctx.m_local->get_velocity( ).m_z ) == 0 || g_prediction.backup_data.m_flags & fl_onground ) {
		m_edgebug_data.m_will_edgebug = false;
		m_edgebug_data.m_will_fail    = true;
	} else if ( g_prediction.backup_data.m_velocity.m_z < -6.f && g_ctx.m_local->get_velocity( ).m_z > g_prediction.backup_data.m_velocity.m_z &&
	            g_ctx.m_local->get_velocity( ).m_z < -6.f && !( g_ctx.m_local->get_flags( ) & fl_onground ) &&
	            g_prediction.backup_data.m_origin.m_z > g_ctx.m_local->get_abs_origin( ).m_z ) {
		const auto gravity = g_convars[ HASH_BT( "sv_gravity" ) ]->get_float( );

		if ( std::floor( g_prediction.backup_data.m_velocity.m_z ) < -7 && std::floor( g_ctx.m_local->get_velocity( ).m_z ) == -7 &&
		     g_ctx.m_local->get_velocity( ).length_2d( ) >= g_prediction.backup_data.m_velocity.length_2d( ) ) {
			m_edgebug_data.m_will_edgebug = true;
			m_edgebug_data.m_will_fail    = false;
		} else {
			float previous_velocity = g_ctx.m_local->get_velocity( ).m_z;

			g_prediction.begin( g_ctx.m_local, cmd );
			g_prediction.end( g_ctx.m_local );

			float expected_vertical_velocity = std::roundf( ( -gravity ) * g_interfaces.m_global_vars_base->m_interval_per_tick + previous_velocity );

			m_edgebug_data.m_will_edgebug = expected_vertical_velocity == std::roundf( g_ctx.m_local->get_velocity( ).m_z );
			m_edgebug_data.m_will_fail    = !( expected_vertical_velocity == std::roundf( g_ctx.m_local->get_velocity( ).m_z ) );
		}
	}
}

void n_movement::impl_t::edgebug_data_t::reset( )
{
	g_movement.m_edgebug_data.m_edgebug_method = edgebug_type_t::eb_standing;
	g_movement.m_edgebug_data.m_last_tick      = 0;
	g_movement.m_edgebug_data.m_ticks_to_stop  = 0;
	g_movement.m_edgebug_data.m_will_edgebug   = false;
	g_movement.m_edgebug_data.m_will_fail      = false;
}

void n_movement::impl_t::autoduck_data_t::reset( )
{
	g_movement.m_autoduck_data.m_did_land_ducking  = false;
	g_movement.m_autoduck_data.m_did_land_standing = false;
	g_movement.m_autoduck_data.m_ducking_vert      = 0.f;
	g_movement.m_autoduck_data.m_standing_vert     = 0.f;
}

void n_movement::impl_t::pixelsurf_data_t::reset( )
{
	g_movement.m_pixelsurf_data.m_predicted_succesful = false;
	g_movement.m_pixelsurf_data.m_in_pixel_surf       = false;
	g_movement.m_pixelsurf_data.m_should_duck         = false;
	g_movement.m_pixelsurf_data.m_prediction_ticks    = 0;
}

void n_movement::impl_t::movement_fix( const c_angle& old_view_point )
{
	c_vector forward = { }, right = { }, up = { };
	g_math.angle_vectors( old_view_point, &forward, &right, &up );

	forward.m_z = right.m_z = up.m_x = up.m_y = 0.f;

	forward.normalize_in_place( );
	right.normalize_in_place( );
	up.normalize_in_place( );

	c_vector old_forward = { }, old_right = { }, old_up = { };
	g_math.angle_vectors( g_ctx.m_cmd->m_view_point, &old_forward, &old_right, &old_up );

	old_forward.m_z = old_right.m_z = old_up.m_x = old_up.m_y = 0.f;

	old_forward.normalize_in_place( );
	old_right.normalize_in_place( );
	old_up.normalize_in_place( );

	const float pitch_forward = forward.m_x * g_ctx.m_cmd->m_forward_move;
	const float yaw_forward   = forward.m_y * g_ctx.m_cmd->m_forward_move;
	const float pitch_side    = right.m_x * g_ctx.m_cmd->m_side_move;
	const float yaw_side      = right.m_y * g_ctx.m_cmd->m_side_move;
	const float roll_up       = up.m_z * g_ctx.m_cmd->m_up_move;

	const float x = old_forward.m_x * pitch_side + old_forward.m_y * yaw_side + old_forward.m_x * pitch_forward + old_forward.m_y * yaw_forward +
	                old_forward.m_z * roll_up;
	const float y =
		old_right.m_x * pitch_side + old_right.m_y * yaw_side + old_right.m_x * pitch_forward + old_right.m_y * yaw_forward + old_right.m_z * roll_up;

	const auto cl_forwardspeed = g_convars[ HASH_BT( "cl_forwardspeed" ) ]->get_float( );
	const auto cl_sidespeed    = g_convars[ HASH_BT( "cl_sidespeed" ) ]->get_float( );

	g_ctx.m_cmd->m_forward_move = std::clamp( x, -cl_forwardspeed, cl_forwardspeed );
	g_ctx.m_cmd->m_side_move    = std::clamp( y, -cl_sidespeed, cl_sidespeed );
}
