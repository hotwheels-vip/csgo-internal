#include "movement.h"
#include "../../../utils/keybinds/keybinds.h"
#include "../../menu/config/config.h"
#include "../../prediction/prediction.h"

void movement::impl::pre_prediction::think( )
{
	if ( !g_ctx.local->is_alive( ) || !g_interfaces.engine->is_fully_connected( ) )
		return;

	if ( !g_ctx.cmd->buttons.has( sdk::IN_BULLRUSH ) && g_config.find< bool >( HASH_BT( "m_fast_duck" ) ) )
		g_ctx.cmd->buttons.add( sdk::IN_BULLRUSH );

	g_movement.bhop( );
}

void movement::impl::post_prediction::think( )
{
	if ( !g_ctx.local->is_alive( ) || !g_interfaces.engine->is_fully_connected( ) )
		return;

	g_movement.edge_jump( );

	g_movement.jump_bug( );

	g_movement.mini_jump( );

	g_movement.auto_align( );
}

void movement::impl::movement_fix( sdk::c_user_cmd* cmd, math::vec3 old_view_angle )
{
	cmd->view_angles = cmd->view_angles.normalize( );
}

void movement::impl::edge_jump( )
{
	if ( !g_config.find< bool >( HASH_BT( "m_ej" ) ) ||
	     !g_input.key_state( input::key_state_t::KEY_DOWN, g_config.find< int >( HASH_BT( "m_ej_key" ) ) ) ) {
		g_movement.longjump.start_timer = false;
		g_movement.longjump.time_stamp  = 0;

		return;
	}

	if ( g_prediction.backup_vars.flags.has( sdk::flags::ONGROUND ) && !g_ctx.local->flags( ).has( sdk::flags::ONGROUND ) ) {
		g_ctx.cmd->buttons.add( sdk::buttons::IN_JUMP );

		g_movement.longjump.start_timer = true;
		g_movement.longjump.time_stamp  = g_interfaces.globals->tick_count;
	}

	[[likely]] if ( g_config.find< bool >( HASH_BT( "m_lj" ) ) )
	{
		if ( g_movement.longjump.start_timer ) {
			if ( g_movement.longjump.time_stamp + 2 > g_interfaces.globals->tick_count ) {
				// did lj = true
				g_ctx.cmd->buttons.add( sdk::buttons::IN_DUCK );
				// :tf: (temporary)
				g_ctx.cmd->forward_move = 0;
			} else {
				// did lj = false
				g_movement.longjump.start_timer = false;
			}
		}
	}

	if ( g_ctx.cmd->buttons.has( sdk::buttons::IN_DUCK ) && !g_ctx.cmd->buttons.has( sdk::buttons::IN_JUMP ) &&
	     g_prediction.backup_vars.flags.has( sdk::flags::ONGROUND ) )
		g_ctx.cmd->buttons.remove( sdk::buttons::IN_DUCK );
}

void movement::impl::jump_bug( )
{
	if ( !g_config.find< bool >( HASH_BT( "m_jb" ) ) )
		return;

	if ( !g_input.key_state( input::key_state_t::KEY_DOWN, g_config.find< int >( HASH_BT( "m_jb_key" ) ) ) )
		return;

	[[unlikely]] if ( g_ctx.local->move_type( ).has_any_of(
						  { sdk::move_type::MOVE_NOCLIP, sdk::move_type::MOVE_LADDER, sdk::move_type::MOVE_FLY } ) ) return;

	[[unlikely]] if ( !( g_ctx.cmd->buttons.has( sdk::buttons::IN_JUMP ) ) )
	{
		static bool ducked = false;

		if ( g_ctx.local->flags( ).has( sdk::flags::ONGROUND ) && !g_prediction.backup_vars.flags.has( sdk::flags::ONGROUND ) && !ducked ) {
			g_ctx.cmd->buttons.add( sdk::buttons::IN_DUCK );

			ducked = true;
		} else
			ducked = false;

		// if player onground and somehow ducked is still true, reset

		if ( g_prediction.backup_vars.flags.has( sdk::flags::ONGROUND ) && ducked )
			ducked = false;
	}
	else // perform normal jb.
	{
		if ( g_ctx.local->flags( ).has( sdk::flags::ONGROUND ) ) {
			if ( !g_prediction.backup_vars.flags.has( sdk::flags::ONGROUND ) )
				g_ctx.cmd->buttons.add( sdk::buttons::IN_DUCK );

			g_ctx.cmd->buttons.remove( sdk::buttons::IN_JUMP );
		}

		// secret sauce
		if ( !g_ctx.local->flags( ).has( sdk::flags::ONGROUND ) && g_prediction.backup_vars.flags.has( sdk::flags::ONGROUND ) )
			g_ctx.cmd->buttons.remove( sdk::buttons::IN_DUCK );
	}
}

void movement::impl::mini_jump( )
{
	if ( !g_config.find< bool >( HASH_BT( "m_mj" ) ) )
		return;

	if ( !g_input.key_state( input::key_state_t::KEY_DOWN, g_config.find< int >( HASH_BT( "m_mj_key" ) ) ) )
		return;

	[[unlikely]] if ( g_ctx.local->move_type( ).has_any_of(
						  { sdk::move_type::MOVE_NOCLIP, sdk::move_type::MOVE_LADDER, sdk::move_type::MOVE_FLY } ) ) return;

	if ( g_prediction.backup_vars.flags.has( sdk::flags::ONGROUND ) && !g_ctx.local->flags( ).has( sdk::flags::ONGROUND ) ) {
		g_ctx.cmd->buttons.add( sdk::buttons::IN_DUCK | sdk::buttons::IN_JUMP );
	}
}

void movement::impl::bhop( )
{
	// its a bhop hack
	[[unlikely]] if ( !g_config.find< bool >( HASH_BT( "m_bh" ) ) ) return;

	if ( g_config.find< bool >( HASH_BT( "m_jb" ) ) &&
	     g_input.key_state( input::key_state_t::KEY_DOWN, g_config.find< int >( HASH_BT( "m_jb_key" ) ) ) )
		return;

	// will return if player is noclip/ladder/fly mode
	[[unlikely]] if ( g_ctx.local->move_type( ).has_any_of(
						  { sdk::move_type::MOVE_NOCLIP, sdk::move_type::MOVE_LADDER, sdk::move_type::MOVE_FLY } ) ) return;

	// remove jump flag if player in air
	if ( !g_ctx.local->flags( ).has( sdk::flags::ONGROUND ) )
		g_ctx.cmd->buttons.remove( sdk::buttons::IN_JUMP );
}

void movement::impl::rotate_movement( math::vec3& angle )
{
	if ( angle.is_zero( ) )
		angle = g_interfaces.engine->get_view_angles( );

	math::vec3 vec_move( g_ctx.cmd->forward_move, g_ctx.cmd->side_move, g_ctx.cmd->up_move );

	math::vec3 ang_move{ };
	math::vector_angles( vec_move, ang_move );

	const float speed = vec_move.length_2d( );

	const float rotation = math::deg2rad( g_ctx.cmd->view_angles.y - angle.y + ang_move.y );

	g_ctx.cmd->forward_move = std::cosf( rotation ) * speed;
	g_ctx.cmd->side_move    = std::sinf( rotation ) * speed;
}

void movement::impl::auto_align( )
{
	// credits patoke
	if ( !g_config.find< bool >( HASH_BT( "m_auto_align" ) ) )
		return;

	// will return if player is noclip/ladder/fly mode
	[[unlikely]] if ( g_ctx.local->move_type( ).has_any_of(
						  { sdk::move_type::MOVE_NOCLIP, sdk::move_type::MOVE_LADDER, sdk::move_type::MOVE_FLY } ) ) return;

	if ( g_prediction.backup_vars.flags.has( sdk::flags::ONGROUND ) || g_prediction.backup_vars.velocity.length_2d( ) < 10.f )
		return;

	const math::vec3 origin   = g_ctx.local->get_abs_origin( );
	const math::vec3 velocity = g_ctx.local->velocity( );

	auto is_colliding = []( ) -> int {
		constexpr float trace_additive = movement::distance_till_adjust + ( 32.f / 2.f );

		const math::vec2< float > ray_differences[ 4 ] = { math::vec2( trace_additive, 0.f ), math::vec2( 0.f, trace_additive ),
			                                               math::vec2( -trace_additive, 0.f ), math::vec2( 0.f, -trace_additive ) };

		int wall_hit = 0;

		for ( int i = 1; i <= 4; i++ ) {
			sdk::c_game_trace trace;
			sdk::c_trace_filter filter;
			filter.skip = g_ctx.local;

			math::vec3 to = g_ctx.local->get_abs_origin( ) + math::vec3( ray_differences[ i - 1 ].x, ray_differences[ i - 1 ].y, 0 );
			sdk::ray_t ray{ };
			ray.init( g_ctx.local->get_abs_origin( ), to );
			g_interfaces.engine_trace->trace_ray( ray, sdk::masks::MASK_ALL, &filter, &trace );

			if ( trace.did_hit( ) && !trace.entity->is_player( ) )
				wall_hit = i;
		}

		return wall_hit;
	};

	const auto has_to_align = []( ) -> bool {
		const auto origin = g_ctx.local->get_abs_origin( );

		const math::vec2 remainder1 = math::vec2( 1.f - ( origin.x - floor( origin.x ) ), 1.f - ( origin.y - floor( origin.y ) ) );
		const math::vec2 remainder2 = math::vec2( ( origin.x - floor( origin.x ) ), ( origin.y - floor( origin.y ) ) );

		return ( ( remainder1.x >= movement::distance_to_stop && remainder1.x <= movement::distance_till_adjust ) ||
		         ( remainder1.y >= movement::distance_to_stop && remainder1.y <= movement::distance_till_adjust ) ) ||
		       ( ( remainder2.x >= movement::distance_to_stop && remainder2.x <= movement::distance_till_adjust ) ||
		         ( remainder2.y >= movement::distance_to_stop && remainder2.y <= movement::distance_till_adjust ) );
	};

	if ( !has_to_align( ) )
		return;

	int wall_collided = is_colliding( );
	if ( wall_collided != 0 ) {
		int is_wall_parallel = wall_collided == 1 || wall_collided == 2 ? 1 : -1;

		math::vec3 velocity_angle = math::calculate_angle( origin, origin + velocity );

		float base_yaw = math::normalize_yaw( velocity_angle.y );

		int direction =
			velocity_angle.y == 0.f ? -1 * is_wall_parallel : round( math::normalize_yaw( velocity_angle.y ) / ( 180.f * is_wall_parallel ) );

		float angle_diff = math::rad2deg( atan( ( g_ctx.cmd->buttons.has( sdk::buttons::IN_DUCK ) ? 4.6775f : 4.5500f ) / velocity.length_2d( ) ) ) *
		                   ( 2.f * math::util::pi_f );

		math::vec3 angle = { g_ctx.cmd->view_angles.x, math::normalize_yaw( base_yaw + direction * angle_diff ), g_ctx.cmd->view_angles.z };

		g_movement.rotate_movement( angle );
	}
}
