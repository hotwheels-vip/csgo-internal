#include "movement.h"
#include "../../convars/convars.h"
#include "../../includes.h"
#include "../../source_engine/enumerations/e_flags.h"
#include "../../source_engine/enumerations/e_move_types.h"
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
}
