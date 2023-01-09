#include "movement.h"
#include "../../convars/convars.h"
#include "../../includes.h"
#include "../../source_engine/enumerations/e_flags.h"
#include "../../source_engine/enumerations/e_move_types.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../prediction/prediction.h"

void movement_t::on_create_move_pre( )
{
	prediction.m_data.m_flags    = globals.m_local->flags( );
	prediction.m_data.m_velocity = globals.m_local->velocity( );

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
	const auto flags    = globals.m_local->flags( );
	const auto velocity = globals.m_local->velocity( );

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

		if ( !( globals.m_cmd->m_buttons & e_buttons::in_jump ) ) {
			static bool ducked = false;

			if ( flags & e_flags::fl_onground && !( prediction.m_data.m_flags & e_flags::fl_onground ) && !ducked ) {
				globals.m_cmd->m_buttons |= e_buttons::in_duck;
				ducked = true;
			} else
				ducked = false;

			if ( prediction.m_data.m_flags & e_flags::fl_onground && ducked )
				ducked = false;
		} else {
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
		const float max_forward_speed = convars.find( fnv1a::hash_const( "cl_forwardspeed" ) )->get_float( );
		const float max_side_speed    = convars.find( fnv1a::hash_const( "cl_sidespeed" ) )->get_float( );
		const float max_up_speed      = convars.find( fnv1a::hash_const( "cl_upspeed" ) )->get_float( );

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
		globals.m_cmd->m_up_move      = std::clamp( z, -max_up_speed, max_up_speed );
	}( );
}
