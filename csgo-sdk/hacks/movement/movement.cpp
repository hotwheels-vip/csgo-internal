#include "movement.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

void n_movement::impl_t::on_create_move_post( )
{
	if ( GET_VARIABLE( g_variables.m_auto_align, bool ) )
		this->auto_align( );
}

void n_movement::impl_t::auto_align() {
	auto rotate_movement = []( c_angle& ang ) -> void {
		c_vector vec_move = c_vector( g_ctx.m_cmd->m_forward_move, g_ctx.m_cmd->m_side_move, g_ctx.m_cmd->m_up_move );

		const float speed = vec_move.length_2d( );

		const float rotation = deg2rad( g_ctx.m_cmd->m_view_point.m_y - ang.m_y );

		g_ctx.m_cmd->m_forward_move = std::cosf( rotation ) * speed;
		g_ctx.m_cmd->m_side_move    = std::sinf( rotation ) * speed;
	};

	const c_vector origin   = g_ctx.m_local->get_origin( );
	const c_vector velocity = g_ctx.m_local->get_velocity( );

	constexpr float distance_till_adjust = 0.03125f;
	auto get_colliding_wall              = []( trace_t& out_trace ) -> bool {
        constexpr float trace_additive = distance_till_adjust + ( 32.f / 2.f );

        const c_vector_2d ray_differences[ 4 ] = { c_vector_2d( trace_additive, 0.f ), c_vector_2d( 0.f, trace_additive ),
                                                   c_vector_2d( -trace_additive, 0.f ), c_vector_2d( 0.f, -trace_additive ) };

        int walls_hit = 0;
        for ( int i = 1; i <= 4; i++ ) {
            if ( walls_hit > 1 )
                break;

            trace_t trace{ };
            c_trace_filter filter( g_ctx.m_local );

            c_vector to = g_ctx.m_local->get_origin( ) + c_vector( ray_differences[ i - 1 ].m_x, ray_differences[ i - 1 ].m_y, 0 );
            ray_t ray( g_ctx.m_local->get_origin( ), to );
            g_interfaces.m_engine_trace->trace_ray( ray, 0xFFFFFFFF /* MASK_ALL */, &filter, &trace );

            if ( trace.did_hit( ) && !trace.m_hit_entity->is_player( ) ) {
                walls_hit++;
                out_trace = trace;
            }
        }

        return walls_hit == 1;
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

	if ( velocity.length_2d( ) == 0.f || g_ctx.m_local->get_flags( ) & e_flags::fl_onground || !has_to_align( origin ) )
		return;

	trace_t hit_trace;
	bool is_valid = get_colliding_wall( hit_trace );

	if ( !is_valid )
		return;

	float gain_fraction = g_ctx.m_cmd->m_buttons & e_command_buttons::in_duck ? 4.6775f : 4.5500f;
	float minimum_gain  = rad2deg( atan( gain_fraction / velocity.length_2d( ) ) ) * ( 2.f * std::numbers::pi_v< float > );

	float strafe_yaw = c_vector( hit_trace.m_plane.m_normal ).to_angle( ).flip( ).m_y;

	float yaw_delta = g_math.normalize_angle( velocity.to_angle( ).m_y - strafe_yaw );

	strafe_yaw += yaw_delta < 0.f ? -minimum_gain : minimum_gain;

	c_angle strafe_angle = c_angle( g_ctx.m_cmd->m_view_point.m_x, strafe_yaw, g_ctx.m_cmd->m_view_point.m_z );
	rotate_movement( strafe_angle );
}