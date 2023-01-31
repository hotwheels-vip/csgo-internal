#include "movement.h"

void movement_t::on_create_move_pre( )
{
	prediction.m_data.m_flags    = globals.m_local->flags( );
	prediction.m_data.m_velocity = globals.m_local->velocity( );
	prediction.m_data.m_origin   = globals.m_local->abs_origin( );
	prediction.m_data.m_movetype = globals.m_local->move_type( );

	// no crouch cooldown
	[]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_fast_duck ) )
			return;

		globals.m_cmd->m_buttons |= e_buttons::in_bullrush;
	}( );

	const auto move_type = globals.m_local->move_type( );
	if ( move_type == e_move_types::move_type_ladder || move_type == e_move_types::move_type_noclip || move_type == e_move_types::move_type_fly ||
	     move_type == e_move_types::move_type_observer )
		return;

	// bhop
	[]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_bunny_hop ) )
			return;

		// dont run bhop while jbing
		if ( GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_jump_bug_key ) ) )
			return;

		// dont run bhop while delayhopping
		if ( GET_CONFIG_BOOL( variables.m_movement.m_delay_hop ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_delay_hop_key ) ) )
			return;

		if ( !( globals.m_local->flags( ) & e_flags::fl_onground ) )
			globals.m_cmd->m_buttons &= ~e_buttons::in_jump;
	}( );
}

void movement_t::on_create_move_post( )
{
	const auto move_type = globals.m_local->move_type( );
	if ( move_type == e_move_types::move_type_ladder || move_type == e_move_types::move_type_noclip ||
	     move_type == e_move_types::move_type_observer ) {
		movement.m_edgebug_data.m_will_edgebug = false;
		movement.m_edgebug_data.m_will_fail    = true;
		return;
	}

	/* TODO ~ float ~ store these somewhere to be accessed globally */
	const auto flags    = globals.m_local->flags( );
	const auto velocity = globals.m_local->velocity( );
	const auto origin   = globals.m_local->origin( );

	const auto max_forward_speed = convars.find( fnv1a::hash_const( "cl_forwardspeed" ) )->get_float( );
	const auto max_side_speed    = convars.find( fnv1a::hash_const( "cl_sidespeed" ) )->get_float( );
	const auto gravity           = convars.find( fnv1a::hash_const( "sv_gravity" ) )->get_float( );

	const float movement_angle = movement.direction_yaw( );

	// delay hop
	[ & ]( bool can_delay_hop ) {
		if ( !can_delay_hop )
			return;

		static int ticks;

		if ( flags & e_flags::fl_onground )
			ticks++;
		else
			ticks = 0;

		if ( ticks >= 4 && flags & e_flags::fl_onground )
			globals.m_cmd->m_buttons |= e_buttons::in_jump;
		else
			globals.m_cmd->m_buttons &= ~e_buttons::in_jump;
	}( GET_CONFIG_BOOL( variables.m_movement.m_delay_hop ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_delay_hop_key ) ) );

	// edgejump
	[ & ]( bool can_edgejump ) {
		if ( !can_edgejump )
			return;

		if ( ( prediction.m_data.m_flags & e_flags::fl_onground ) && !( flags & e_flags::fl_onground ) )
			globals.m_cmd->m_buttons |= e_buttons::in_jump;
	}( GET_CONFIG_BOOL( variables.m_movement.m_edge_jump ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_edge_jump_key ) ) );

	// longjump
	[ & ]( ) {
		static int saved_tick = 0;

		if ( !GET_CONFIG_BOOL( variables.m_movement.m_long_jump ) )
			return;

		if ( !input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_long_jump_key ) ) ) {
			saved_tick = 0;
			return;
		}

		if ( flags & e_flags::fl_onground )
			saved_tick = memory.m_globals->m_tick_count;

		if ( !( memory.m_globals->m_tick_count - saved_tick > 2 ) && !( flags & e_flags::fl_onground ) )
			globals.m_cmd->m_buttons |= e_buttons::in_duck;
	}( );

	// minijump
	[ & ]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_mini_jump ) )
			return;

		if ( !input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_mini_jump_key ) ) )
			return;

		if ( prediction.m_data.m_flags & e_flags::fl_onground && !( flags & e_flags::fl_onground ) ) {
			globals.m_cmd->m_buttons |= e_buttons::in_jump;
			globals.m_cmd->m_buttons |= e_buttons::in_duck;
		}
	}( );

	// jumpbug
	[ & ]( bool can_jump_bug ) {
		if ( !can_jump_bug )
			return;

		[[unlikely]] if ( !( globals.m_cmd->m_buttons & e_buttons::in_jump ) )
		{
			static bool ducked = false;

			if ( flags & e_flags::fl_onground && !( prediction.m_data.m_flags & e_flags::fl_onground ) && !ducked ) {
				globals.m_cmd->m_buttons |= e_buttons::in_duck;
				ducked = true;
			} else
				ducked = false;

			if ( prediction.m_data.m_flags & e_flags::fl_onground && ducked )
				ducked = false;
		}
		else
		{
			if ( flags & e_flags::fl_onground && !( prediction.m_data.m_flags & e_flags::fl_onground ) )
				globals.m_cmd->m_buttons |= e_buttons::in_duck;

			if ( flags & e_flags::fl_onground )
				globals.m_cmd->m_buttons &= ~e_buttons::in_jump;

			// remove duck flag once on ground, this also makes 1tick jb(gain extra height on crouched bhops)
			if ( !( flags & fl_onground ) && prediction.m_data.m_flags & fl_onground )
				globals.m_cmd->m_buttons &= ~e_buttons::in_duck;
		}
	}( GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_jump_bug_key ) ) );

	// auto strafe
	[ & ]( bool can_autostrafe ) {
		if ( !can_autostrafe )
			return;

		if ( movement.m_edgebug_data.m_will_edgebug || movement.m_pixelsurf_data.m_in_pixel_surf || prediction.m_data.m_flags & e_flags::fl_onground )
			return;

		c_angle angle{ };
		interfaces.m_engine->get_view_angles( angle );

		movement.strafe_to_yaw( angle, movement_angle );

		movement.rotate_movement( angle );
	}( GET_CONFIG_BOOL( variables.m_movement.m_autostrafe ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_autostrafe_key ) ) );

	// auto align
	[ & ]( const bool can_auto_align ) {
		if ( prediction.m_data.m_flags & e_flags::fl_onground || movement.m_pixelsurf_data.m_in_pixel_surf ||
		     movement.m_edgebug_data.m_will_edgebug || !can_auto_align )
			return;

		auto_align( globals.m_cmd, max_forward_speed, max_side_speed );
	}( GET_CONFIG_BOOL( variables.m_movement.m_auto_align ) );

	// pixelsurf (logic)
	const float target_ps_velocity = gravity * 0.5f * memory.m_globals->m_interval_per_tick;
	[ & ]( bool can_pixelsurf ) {
		if ( can_pixelsurf ) {
			if ( movement.m_pixelsurf_data.m_in_pixel_surf || globals.m_local->flags( ) & e_flags::fl_onground ||
			     globals.m_local->velocity( ).m_z >= 0.f )
				return;

			if ( !movement.m_pixelsurf_data.m_predicted_succesful ) {
				movement.m_pixelsurf_data.m_should_duck = false;
				for ( int prediction_type = 0; prediction_type < 2; prediction_type++ ) {
					if ( movement.m_pixelsurf_data.m_in_pixel_surf )
						break;

					prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );

					int backup_flags         = globals.m_local->flags( );
					c_vector backup_velocity = globals.m_local->velocity( );

					for ( int i = 0; i < 12; i++ ) {
						c_user_cmd* simulated_cmd = new c_user_cmd( *globals.m_cmd );

						if ( prediction_type == 0 )
							simulated_cmd->m_buttons |= e_buttons::in_duck;
						else
							simulated_cmd->m_buttons &= ~e_buttons::in_duck;

						auto_align( simulated_cmd, max_forward_speed, max_side_speed );

						prediction.begin( simulated_cmd );
						prediction.end( );

						if ( backup_flags & e_flags::fl_onground )
							break;

						if ( globals.m_local->velocity( ).m_z == -target_ps_velocity && backup_velocity.m_z == -target_ps_velocity ) {
							movement.m_pixelsurf_data.m_prediction_ticks = globals.m_cmd->m_tick_count + i;
							movement.m_pixelsurf_data.m_simulated_cmd    = simulated_cmd;
							movement.m_pixelsurf_data.m_should_duck      = !( prediction_type == 1 );
							movement.m_pixelsurf_data.m_in_pixel_surf    = true;
							break;
						}

						backup_velocity = globals.m_local->velocity( );
						backup_flags    = globals.m_local->flags( );

						delete simulated_cmd;
					}

					prediction.begin( globals.m_cmd );
					prediction.end( );

					prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );
				}
			}

		} else
			movement.m_pixelsurf_data.reset( );
	}( GET_CONFIG_BOOL( variables.m_movement.m_pixel_surf ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_pixel_surf_key ) ) );

	// auto duck
	[ & ]( const bool can_auto_duck ) {
		// i know this is a mess but its preventing it from ducking in useless ways.

		if ( !can_auto_duck || prediction.m_data.m_flags & e_flags::fl_onground || movement.m_edgebug_data.m_will_edgebug ||
		     movement.m_pixelsurf_data.m_in_pixel_surf ||
		     ( GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) &&
		       input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_jump_bug_key ) ) ) ) {
			movement.m_autoduck_data.reset( );

			return;
		}

		prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );

		for ( int i = 0; i < 2; i++ ) {
			if ( globals.m_local->flags( ) & e_flags::fl_onground )
				break;

			c_user_cmd* simulated_cmd = new c_user_cmd( *globals.m_cmd );

			simulated_cmd->m_buttons |= e_buttons::in_duck;

			prediction.begin( simulated_cmd );
			prediction.end( );

			if ( globals.m_local->flags( ) & e_flags::fl_onground ) {
				movement.m_autoduck_data.m_did_land_ducking = true;
				movement.m_autoduck_data.m_ducking_vert     = globals.m_local->origin( ).m_z;
				break;
			}
			delete simulated_cmd;
		}

		prediction.begin( globals.m_cmd );
		prediction.end( );

		prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );

		if ( !movement.m_autoduck_data.m_did_land_ducking )
			return;

		for ( int i = 0; i < 2; i++ ) {
			if ( globals.m_local->flags( ) & e_flags::fl_onground )
				break;

			c_user_cmd* simulated_cmd = new c_user_cmd( *globals.m_cmd );

			simulated_cmd->m_buttons &= ~e_buttons::in_duck;

			prediction.begin( simulated_cmd );
			prediction.end( );

			if ( globals.m_local->flags( ) & e_flags::fl_onground ) {
				movement.m_autoduck_data.m_did_land_standing = true;
				movement.m_autoduck_data.m_standing_vert     = globals.m_local->origin( ).m_z;
				break;
			}
			delete simulated_cmd;
		}

		prediction.begin( globals.m_cmd );
		prediction.end( );

		prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );

		if ( movement.m_autoduck_data.m_did_land_ducking && movement.m_autoduck_data.m_did_land_standing ) {
			if ( movement.m_autoduck_data.m_ducking_vert > movement.m_autoduck_data.m_standing_vert )
				globals.m_cmd->m_buttons |= e_buttons::in_duck;
		} else if ( movement.m_autoduck_data.m_did_land_ducking && !movement.m_autoduck_data.m_did_land_standing )
			globals.m_cmd->m_buttons |= e_buttons::in_duck;
	}( GET_CONFIG_BOOL( variables.m_movement.m_autoduck ) );

	// movement correction
	[ & ]( ) {
		c_vector forward = { }, right = { }, up = { };
		mathematics.angle_vectors( globals.m_old_view_point, &forward, &right, &up );

		forward.m_z = right.m_z = up.m_x = up.m_y = 0.f;

		forward.normalize_in_place( );
		right.normalize_in_place( );
		up.normalize_in_place( );

		c_vector old_forward = { }, old_right = { }, old_up = { };
		mathematics.angle_vectors( globals.m_cmd->m_view_point, &old_forward, &old_right, &old_up );

		old_forward.m_z = old_right.m_z = old_up.m_x = old_up.m_y = 0.f;

		old_forward.normalize_in_place( );
		old_right.normalize_in_place( );
		old_up.normalize_in_place( );

		const float pitch_forward = forward.m_x * globals.m_cmd->m_forward_move;
		const float yaw_forward   = forward.m_y * globals.m_cmd->m_forward_move;
		const float pitch_side    = right.m_x * globals.m_cmd->m_side_move;
		const float yaw_side      = right.m_y * globals.m_cmd->m_side_move;
		const float roll_up       = up.m_z * globals.m_cmd->m_up_move;

		const float x = old_forward.m_x * pitch_side + old_forward.m_y * yaw_side + old_forward.m_x * pitch_forward + old_forward.m_y * yaw_forward +
		                old_forward.m_z * roll_up;
		const float y = old_right.m_x * pitch_side + old_right.m_y * yaw_side + old_right.m_x * pitch_forward + old_right.m_y * yaw_forward +
		                old_right.m_z * roll_up;

		globals.m_cmd->m_forward_move = std::clamp( x, -max_forward_speed, max_forward_speed );
		globals.m_cmd->m_side_move    = std::clamp( y, -max_side_speed, max_side_speed );
	}( );

	// pixel surf (locking)
	[ & ]( ) {
		if ( movement.m_pixelsurf_data.m_in_pixel_surf ) {
			prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );

			if ( !movement.m_pixelsurf_data.m_predicted_succesful ) {
				if ( movement.m_pixelsurf_data.m_prediction_ticks < globals.m_cmd->m_tick_count )
					movement.m_pixelsurf_data.m_predicted_succesful = true;

				globals.m_cmd->m_buttons = movement.m_pixelsurf_data.m_simulated_cmd->m_buttons;
				// globals.m_cmd->m_view_point   = movement.m_pixelsurf_data.m_simulated_cmd->m_view_point;
				globals.m_cmd->m_side_move    = movement.m_pixelsurf_data.m_simulated_cmd->m_side_move;
				globals.m_cmd->m_forward_move = movement.m_pixelsurf_data.m_simulated_cmd->m_forward_move;

				movement.rotate_movement( movement.m_pixelsurf_data.m_simulated_cmd->m_view_point );
			}

			if ( movement.m_pixelsurf_data.m_should_duck )
				globals.m_cmd->m_buttons |= e_buttons::in_duck;
			else
				globals.m_cmd->m_buttons &= ~e_buttons::in_duck;

			if ( movement.m_pixelsurf_data.m_predicted_succesful && !( globals.m_local->velocity( ).m_z == -target_ps_velocity ) ) {
				movement.m_pixelsurf_data.m_in_pixel_surf       = false;
				movement.m_pixelsurf_data.m_predicted_succesful = false;
				return;
			}
			return;
		}
	}( );

	// edgebug
	[ & ]( bool can_edgebug ) {
		if ( !can_edgebug || movement.m_pixelsurf_data.m_in_pixel_surf ) {
			m_edgebug_data.reset( );
			return;
		}

		const float yaw_delta = std::clamp( globals.m_cmd->m_view_point.m_y - globals.m_last_tick_yaw,
		                                    -( 180.f / GET_CONFIG_INT( variables.m_movement.m_edge_bug_ticks ) ),
		                                    180.f / GET_CONFIG_INT( variables.m_movement.m_edge_bug_ticks ) );

		const float original_forward_move = globals.m_cmd->m_forward_move;
		const float original_side_move    = globals.m_cmd->m_side_move;
		const auto original_view_point    = globals.m_cmd->m_view_point;

		const auto loop_through_ticks = [ & ]( const bool ducked, const bool strafe = false ) {
			prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );

			movement.m_edgebug_data.m_starting_yaw = original_view_point.m_y;

			for ( int i = 0; i <= GET_CONFIG_INT( variables.m_movement.m_edge_bug_ticks ); i++ ) {
				c_user_cmd* simulated_cmd = new c_user_cmd( *globals.m_cmd );

				if ( ducked ) {
					simulated_cmd->m_buttons |= e_buttons::in_duck;
					globals.m_local->flags( ) |= e_flags::fl_ducking;
				} else {
					simulated_cmd->m_buttons &= ~e_buttons::in_duck;
					globals.m_local->flags( ) &= ~e_flags::fl_ducking;
				}

				if ( !strafe ) {
					m_edgebug_data.m_strafing = false;

					simulated_cmd->m_forward_move = 0;
					simulated_cmd->m_side_move    = 0;

					// p sure these are not needed, but just making sure
					simulated_cmd->m_buttons &= ~e_buttons::in_moveleft;
					simulated_cmd->m_buttons &= ~e_buttons::in_moveright;
					simulated_cmd->m_buttons &= ~e_buttons::in_forward;
					simulated_cmd->m_buttons &= ~e_buttons::in_back;
				} else // strafed
				{
					m_edgebug_data.m_strafing = true;

					simulated_cmd->m_forward_move = original_forward_move;
					simulated_cmd->m_side_move    = original_side_move;

					simulated_cmd->m_view_point.m_y = mathematics.normalize_yaw( original_view_point.m_y + ( yaw_delta * i ) );
				}

				prediction.begin( simulated_cmd );
				prediction.end( );

				if ( prediction.m_data.m_flags & e_flags::fl_onground || round( prediction.m_data.m_velocity.m_z ) >= 0 ||
				     globals.m_local->flags( ) & fl_onground ) {
					m_edgebug_data.m_will_edgebug = false;
					break;
				}

				if ( !m_edgebug_data.m_will_edgebug )
					this->detect_edgebug( simulated_cmd );

				if ( m_edgebug_data.m_will_edgebug ) {
					m_edgebug_data.m_saved_mousedx = abs( simulated_cmd->m_mouse_delta_x );
					m_edgebug_data.m_ticks_to_stop = i + 1;
					m_edgebug_data.m_last_tick     = memory.m_globals->m_tick_count;

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

			prediction.begin( globals.m_cmd );
			prediction.end( );

			prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );
		};

		// non strafed edgebugs
		if ( !m_edgebug_data.m_will_edgebug )
			loop_through_ticks( edgebug_type_t::eb_standing );
		if ( !m_edgebug_data.m_will_edgebug )
			loop_through_ticks( edgebug_type_t::eb_ducking );

		// strafed edgebugs
		if ( GET_CONFIG_BOOL( variables.m_movement.m_advanced_detection ) && yaw_delta < 0.1f ) {
			if ( !m_edgebug_data.m_will_edgebug )
				loop_through_ticks( edgebug_type_t::eb_standing, true );
			if ( !m_edgebug_data.m_will_edgebug )
				loop_through_ticks( edgebug_type_t::eb_ducking, true );
		}

		if ( m_edgebug_data.m_will_edgebug ) {
			prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );

			if ( memory.m_globals->m_tick_count < m_edgebug_data.m_ticks_to_stop + m_edgebug_data.m_last_tick ) {
				if ( m_edgebug_data.m_strafing ) {
					globals.m_cmd->m_side_move    = m_edgebug_data.m_side_move;
					globals.m_cmd->m_forward_move = m_edgebug_data.m_forward_move;

					const float final_yaw =
						mathematics.normalize_yaw( m_edgebug_data.m_starting_yaw +
					                               ( m_edgebug_data.m_yaw_delta * ( memory.m_globals->m_tick_count - m_edgebug_data.m_last_tick ) ) );

					globals.m_cmd->m_view_point.m_y = final_yaw;

				} else {
					globals.m_cmd->m_side_move    = 0.f;
					globals.m_cmd->m_forward_move = 0.f;
					globals.m_cmd->m_buttons &= ~e_buttons::in_moveleft;
					globals.m_cmd->m_buttons &= ~e_buttons::in_moveright;
					globals.m_cmd->m_buttons &= ~e_buttons::in_forward;
					globals.m_cmd->m_buttons &= ~e_buttons::in_back;
				}
				if ( m_edgebug_data.m_edgebug_method == edgebug_type_t::eb_ducking ) {
					globals.m_cmd->m_buttons |= in_duck;
				} else
					globals.m_cmd->m_buttons &= ~in_duck;
			} else
				m_edgebug_data.reset( );
		}
	}( GET_CONFIG_BOOL( variables.m_movement.m_edge_bug ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_edge_bug_key ) ) );
}

// handles strafing to edgebug
// called on frame_stage_notify frame_start
void movement_t::handle_edgebug_view_point( )
{
	if ( !movement.m_edgebug_data.m_will_edgebug || !movement.m_edgebug_data.m_strafing )
		return;

	float final_yaw{ };

	c_angle wish_angles = { globals.m_old_view_point.m_x, movement.m_edgebug_data.m_starting_yaw, globals.m_old_view_point.m_z };

	float hit_time_delta = mathematics.ticks_to_time( movement.m_edgebug_data.m_ticks_to_stop );

	float cur_time_delta = memory.m_globals->m_current_time - mathematics.ticks_to_time( movement.m_edgebug_data.m_last_tick );

	final_yaw = mathematics.normalize_yaw( movement.m_edgebug_data.m_yaw_delta *
	                                       ( movement.m_edgebug_data.m_ticks_to_stop * ( cur_time_delta / hit_time_delta ) ) );

	wish_angles.m_y += final_yaw;

	interfaces.m_engine->set_view_angles( wish_angles );
}

void movement_t::detect_edgebug( c_user_cmd* cmd )
{
	if ( prediction.m_data.m_velocity.m_z > 0 || globals.m_local->move_type( ) & move_type_noclip ||
	     globals.m_local->move_type( ) & move_type_ladder ) {
		m_edgebug_data.m_will_edgebug = false;
		m_edgebug_data.m_will_fail    = true;
		return;
	}

	if ( round( globals.m_local->velocity( ).m_z ) == 0 || prediction.m_data.m_flags & fl_onground ) {
		m_edgebug_data.m_will_edgebug = false;
		m_edgebug_data.m_will_fail    = true;
	} else if ( prediction.m_data.m_velocity.m_z < -6.f && globals.m_local->velocity( ).m_z > prediction.m_data.m_velocity.m_z &&
	            globals.m_local->velocity( ).m_z < -6.f && !( globals.m_local->flags( ) & fl_onground ) &&
	            prediction.m_data.m_origin.m_z > globals.m_local->abs_origin( ).m_z ) {
		const auto gravity = convars.find( fnv1a::hash_const( "sv_gravity" ) )->get_float( );

		if ( std::floor( prediction.m_data.m_velocity.m_z ) < -7 && std::floor( globals.m_local->velocity( ).m_z ) == -7 &&
		     globals.m_local->velocity( ).length_2d( ) >= prediction.m_data.m_velocity.length_2d( ) ) {
			m_edgebug_data.m_will_edgebug = true;
			m_edgebug_data.m_will_fail    = false;
		} else {
			float previous_velocity = globals.m_local->velocity( ).m_z;

			prediction.begin( globals.m_cmd );
			prediction.end( );

			float expected_vertical_velocity = std::roundf( ( -gravity ) * memory.m_globals->m_interval_per_tick + previous_velocity );

			m_edgebug_data.m_will_edgebug = expected_vertical_velocity == std::roundf( globals.m_local->velocity( ).m_z );
			m_edgebug_data.m_will_fail    = !( expected_vertical_velocity == std::roundf( globals.m_local->velocity( ).m_z ) );
		}
	}
}

void movement_t::edgebug_data_t::reset( )
{
	movement.m_edgebug_data.m_edgebug_method = edgebug_type_t::eb_standing;
	movement.m_edgebug_data.m_last_tick      = 0;
	movement.m_edgebug_data.m_ticks_to_stop  = 0;
	movement.m_edgebug_data.m_will_edgebug   = false;
	movement.m_edgebug_data.m_will_fail      = false;
}

void movement_t::autoduck_data_t::reset( )
{
	movement.m_autoduck_data.m_did_land_ducking  = false;
	movement.m_autoduck_data.m_did_land_standing = false;
	movement.m_autoduck_data.m_ducking_vert      = 0.f;
	movement.m_autoduck_data.m_standing_vert     = 0.f;
}

void movement_t::pixelsurf_data_t::reset( )
{
	movement.m_pixelsurf_data.m_predicted_succesful = false;
	movement.m_pixelsurf_data.m_in_pixel_surf       = false;
	movement.m_pixelsurf_data.m_should_duck         = false;
	movement.m_pixelsurf_data.m_prediction_ticks    = 0;
}

void movement_t::rotate_movement( c_angle& angle )
{
	if ( angle.m_x == 0 && angle.m_y == 0 && angle.m_z == 0 )
		interfaces.m_engine->get_view_angles( angle );

	const c_vector movement = { globals.m_cmd->m_forward_move, globals.m_cmd->m_side_move, 0 };

	const c_vector angle_movement = mathematics.vector_angle( movement );

	const float rotation = DEG2RAD( globals.m_cmd->m_view_point.m_y - angle.m_y + angle_movement.m_y );

	globals.m_cmd->m_forward_move = std::cosf( rotation ) * movement.length_2d( );
	globals.m_cmd->m_side_move    = std::sinf( rotation ) * movement.length_2d( );
}

float movement_t::get_perfect_delta( )
{
	float sv_airaccelerate     = convars.find( fnv1a::hash_const( "sv_airaccelerate" ) )->get_float( );
	float sv_air_max_wishspeed = convars.find( fnv1a::hash_const( "sv_air_max_wishspeed" ) )->get_float( );

	if ( !sv_airaccelerate )
		sv_airaccelerate = 12;

	if ( !sv_air_max_wishspeed )
		sv_air_max_wishspeed = 30;

	const auto speed       = std::hypotf( globals.m_local->velocity( ).m_x, globals.m_local->velocity( ).m_y );
	const auto accel_speed = std::min( memory.m_globals->m_interval_per_tick * 30 * sv_airaccelerate, sv_air_max_wishspeed );

	return std::atan( accel_speed / speed ) * ( 180 / PI );
}

void movement_t::strafe_to_yaw( c_angle& angle, const float yaw )
{
	const auto max_side_speed = convars.find( fnv1a::hash_const( "cl_sidespeed" ) )->get_float( );

	angle.m_y += yaw;
	globals.m_cmd->m_side_move    = 0.f;
	globals.m_cmd->m_forward_move = 0.f;

	const auto degrees = RAD2DEG( std::atan2f( globals.m_local->velocity( ).m_y, globals.m_local->velocity( ).m_x ) );

	const auto delta = mathematics.normalize_yaw( angle.m_y - degrees );

	globals.m_cmd->m_side_move = delta > 0.f ? -max_side_speed : max_side_speed;
	angle.m_y                  = mathematics.normalize_yaw( angle.m_y - delta );
}

void movement_t::auto_align( c_user_cmd* cmd, float max_fw, float max_sw )
{
	c_game_trace trace{ }, second_trace{ };

	struct {
		c_vector start_pos;
		c_vector end_pos, second_end_pos;
		c_vector current_velocity;
		float velocity    = 0.f;
		float ideal_delta = 0.f;
	} align_info;

	const c_vector wish_direction{ std::cos( DEG2RAD( globals.m_old_view_point.m_y + direction_yaw( ) ) ) * 17.f,
		                           std::sin( DEG2RAD( globals.m_old_view_point.m_y + direction_yaw( ) ) ) * 17.f, 0.f };

	if ( wish_direction.is_zero( ) )
		return;

	// set our start and finish points for engine tracing
	align_info.start_pos = globals.m_local->abs_origin( );
	align_info.end_pos   = align_info.start_pos + wish_direction;

	ray_t ray( align_info.start_pos, align_info.end_pos );

	c_trace_filter filter( globals.m_local );

	// our wish direction trace
	interfaces.m_engine_trace->trace_ray( ray, e_mask::mask_playersolid, &filter, &trace );

	// if trace hit anything
	if ( trace.m_fraction < 1.f && trace.m_plane.m_normal.m_z == 0.f ) {
		// wall angles
		c_vector angles = { trace.m_plane.m_normal.m_x * -16.005f, trace.m_plane.m_normal.m_y * -16.005f, 0.f };

		// initialize our second trace
		align_info.second_end_pos = align_info.start_pos + angles;
		ray_t second_ray( align_info.start_pos, align_info.second_end_pos );
		interfaces.m_engine_trace->trace_ray( second_ray, e_mask::mask_playersolid, &filter, &second_trace );

		if ( trace.m_plane.m_normal.m_x != second_trace.m_plane.m_normal.m_x || trace.m_plane.m_normal.m_y != second_trace.m_plane.m_normal.m_y ||
		     trace.m_plane.m_normal.m_z != second_trace.m_plane.m_normal.m_z ) {
			c_vector angle_to_wall = mathematics.to_angle( angles );

			align_info.velocity    = std::hypotf( globals.m_local->velocity( ).m_x, globals.m_local->velocity( ).m_y );
			align_info.ideal_delta = RAD2DEG( atanf( ( cmd->m_buttons & in_duck ? 4.6775f : 4.5500f ) / align_info.velocity ) ) * ( 2.f * PI );

			align_info.current_velocity     = globals.m_local->velocity( );
			align_info.current_velocity.m_z = 0.f;

			c_vector current_velocity_angle = mathematics.to_angle( align_info.current_velocity );

			c_vector delta = current_velocity_angle - angle_to_wall;

			delta.normalize_in_place( );

			delta.m_y >= 0.f ? angle_to_wall.m_y += align_info.ideal_delta : angle_to_wall.m_y -= align_info.ideal_delta;

			float cos_rot = std::cos( DEG2RAD( angle_to_wall.m_y - globals.m_old_view_point.m_y ) );
			float sin_rot = std::sin( DEG2RAD( angle_to_wall.m_y - globals.m_old_view_point.m_y ) );

			cmd->m_forward_move = cos_rot * max_fw;
			cmd->m_side_move    = -sin_rot * max_sw;
		}
	}
}

const float movement_t::direction_yaw( )
{
	float yaw = 0.f;

	const bool back    = globals.m_cmd->m_buttons & e_buttons::in_back;
	const bool forward = globals.m_cmd->m_buttons & e_buttons::in_forward;
	const bool right   = globals.m_cmd->m_buttons & e_buttons::in_moveleft;
	const bool left    = globals.m_cmd->m_buttons & e_buttons::in_moveright;

	// diagonal rotation based on pressed keys
	if ( back ) {
		yaw = -180.f;

		if ( right )
			yaw -= 45.f;

		else if ( left )
			yaw += 45.f;
	} else if ( right ) {
		yaw = 90.f;

		if ( back )
			yaw += 45.f;

		else if ( forward )
			yaw -= 45.f;
	} else if ( left ) {
		yaw = -90.f;

		if ( back )
			yaw -= 45.f;

		else if ( forward )
			yaw += 45.f;
	} else
		yaw = 0.f;

	return yaw;
}
