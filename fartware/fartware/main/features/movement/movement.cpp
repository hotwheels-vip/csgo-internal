#include "movement.h"

void movement_t::on_create_move_pre( )
{
	prediction.m_data.m_flags    = globals.m_local->flags( );
	prediction.m_data.m_velocity = globals.m_local->velocity( );

	// no crouch cooldown
	[]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_fast_duck ) )
			return;

		globals.m_cmd->m_buttons |= e_buttons::in_bullrush;
	}( );

	const auto move_type = globals.m_local->move_type( );
	if ( move_type == e_move_types::move_type_ladder || move_type == e_move_types::move_type_noclip || move_type == e_move_types::move_type_observer )
		return;

	// bhop
	[]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_bunny_hop ) )
			return;

		if ( GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) && input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_jump_bug_key ) ) )
			return;

		if ( convars.find( fnv1a::hash_const( "sv_autobunnyhopping" ) )->get_int( ) )
			return;

		if ( !( globals.m_local->flags( ) & e_flags::fl_onground ) )
			globals.m_cmd->m_buttons &= ~e_buttons::in_jump;
	}( );
}

void movement_t::on_create_move_post( )
{
	const auto move_type = globals.m_local->move_type( );
	if ( move_type == e_move_types::move_type_ladder || move_type == e_move_types::move_type_noclip || move_type == e_move_types::move_type_observer )
		return;

	/* TODO ~ float ~ store these somewhere to be accessed globally */
	const auto flags             = globals.m_local->flags( );
	const auto velocity          = globals.m_local->velocity( );
	const auto max_forward_speed = convars.find( fnv1a::hash_const( "cl_forwardspeed" ) )->get_float( );
	const auto max_side_speed    = convars.find( fnv1a::hash_const( "cl_sidespeed" ) )->get_float( );

	// edgejump
	[ & ]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_edge_jump ) )
			return;

		if ( !input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_edge_jump_key ) ) )
			return;

		if ( ( prediction.m_data.m_flags & e_flags::fl_onground ) && !( flags & e_flags::fl_onground ) )
			globals.m_cmd->m_buttons |= e_buttons::in_jump;
	}( );

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
	[ & ]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) )
			return;

		if ( !input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_jump_bug_key ) ) )
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
	}( );

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
		const float z =
			old_up.m_x * yaw_side + old_up.m_y * pitch_side + old_up.m_x * yaw_forward + old_up.m_y * pitch_forward + old_up.m_z * roll_up;

		globals.m_cmd->m_forward_move = std::clamp( x, -max_forward_speed, max_forward_speed );
		globals.m_cmd->m_side_move    = std::clamp( y, -max_side_speed, max_side_speed );
	}( );

	// auto align
	[ & ]( ) {
		if ( prediction.m_data.m_flags & e_flags::fl_onground )
			return;

		c_game_trace trace, second_trace;
		ray_t ray, second_ray;
		c_trace_filter filter, second_filter;

		struct {
			c_vector start_pos;
			c_vector end_pos, second_end_pos;
			c_vector current_velocity;
			float movement_angle = 0.f;
			float velocity       = 0.f;
			float ideal_delta    = 0.f;
		} align_info;

		const bool back    = globals.m_cmd->m_buttons & e_buttons::in_back;
		const bool forward = globals.m_cmd->m_buttons & e_buttons::in_forward;
		const bool right   = globals.m_cmd->m_buttons & e_buttons::in_moveleft;
		const bool left    = globals.m_cmd->m_buttons & e_buttons::in_moveright;

		// diagonal rotation based on pressed keys
		if ( back ) {
			align_info.movement_angle = -180.f;

			if ( right )
				align_info.movement_angle -= 45.f;

			else if ( left )
				align_info.movement_angle += 45.f;
		} else if ( right ) {
			align_info.movement_angle = 90.f;

			if ( back )
				align_info.movement_angle += 45.f;

			else if ( forward )
				align_info.movement_angle -= 45.f;
		} else if ( left ) {
			align_info.movement_angle = -90.f;

			if ( back )
				align_info.movement_angle -= 45.f;

			else if ( forward )
				align_info.movement_angle += 45.f;
		} else
			align_info.movement_angle = 0.f;

		const c_vector wish_direction{ std::cos( DEG2RAD( globals.m_cmd->m_view_point.m_y + align_info.movement_angle ) ) * 17.f,
			                           std::sin( DEG2RAD( globals.m_cmd->m_view_point.m_y + align_info.movement_angle ) ) * 17.f, 0.f };

		if ( wish_direction.is_zero( ) )
			return;

		// set our start and finish points for engine tracing
		align_info.start_pos = globals.m_local->abs_origin( );
		align_info.end_pos   = align_info.start_pos + wish_direction;

		ray.init( align_info.start_pos, align_info.end_pos );

		filter.skip = globals.m_local;

		// our wish direction trace
		interfaces.m_engine_trace->trace_ray( ray, masks::MASK_PLAYERSOLID, &filter, &trace );

		// if trace hit anything
		if ( trace.fraction < 1.f && trace.plane.normal.m_z == 0.f ) {
			// wall angles
			c_vector angles = { trace.plane.normal.m_x * -16.005f, trace.plane.normal.m_y * -16.005f, 0.f };

			// initialize our second trace
			align_info.second_end_pos = align_info.start_pos + angles;
			second_ray.init( align_info.start_pos, align_info.second_end_pos );
			interfaces.m_engine_trace->trace_ray( second_ray, masks::MASK_PLAYERSOLID, &filter, &second_trace );

			if ( trace.plane != second_trace.plane ) {
				c_vector angle_to_wall = mathematics.to_angle( angles );

				align_info.velocity = std::hypotf( globals.m_local->velocity( ).m_x, globals.m_local->velocity( ).m_y );
				align_info.ideal_delta =
					RAD2DEG( atanf( ( globals.m_cmd->m_buttons & in_duck ? 4.6775f : 4.5500f ) / align_info.velocity ) ) * ( 2.f * PI );

				align_info.current_velocity     = globals.m_local->velocity( );
				align_info.current_velocity.m_z = 0.f;

				c_vector current_velocity_angle = mathematics.to_angle( align_info.current_velocity );

				c_vector delta = current_velocity_angle - angle_to_wall;

				delta.normalize_in_place( );

				delta.m_y >= 0.f ? angle_to_wall.m_y += align_info.ideal_delta : angle_to_wall.m_y -= align_info.ideal_delta;

				float cos_rot = std::cos( DEG2RAD( angle_to_wall.m_y - globals.m_cmd->m_view_point.m_y ) );
				float sin_rot = std::sin( DEG2RAD( angle_to_wall.m_y - globals.m_cmd->m_view_point.m_y ) );

				globals.m_cmd->m_forward_move = cos_rot * max_forward_speed;
				globals.m_cmd->m_side_move    = -sin_rot * max_side_speed;
			}
		}
	}( );
}

void movement_t::detect_edgebug( ) { }

void movement_t::edgebug( ) { }