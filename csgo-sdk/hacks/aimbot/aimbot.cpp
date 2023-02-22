#include "aimbot.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../entity_cache/entity_cache.h"
#include "../prediction/prediction.h"

c_base_entity* n_aimbot::impl_t::find_closest_player( )
{
	float closest_fov             = 180.f;
	c_base_entity* closest_player = nullptr;

	const auto eye_position = g_ctx.m_local->get_eye_position( false );

	g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
		if ( !entity || !entity->is_alive( ) || entity->is_dormant( ) || !g_ctx.m_local->is_enemy( entity ) || entity == g_ctx.m_local )
			return;

		const auto angle = g_math.calculate_angle( eye_position, entity->get_hitbox_position( hitbox_head ) );

		if ( float calculated_fov = g_math.calculate_fov( g_prediction.backup_data.m_view_angles, angle ); calculated_fov < closest_fov ) {
			closest_fov    = calculated_fov;
			closest_player = entity;
		}
	} );

	return closest_player;
}

void n_aimbot::impl_t::on_create_move_post( )
{
	if ( !GET_VARIABLE( g_variables.m_aimbot_enable, bool ) )
		return;

	if ( g_ctx.m_cmd->m_tick_count == 0 )
		return;
	//
	// static c_angle angles_to_head = { };
	// float current_fov         = 180.f;
	// const auto max_allocation = static_cast< int >( 1.f / g_interfaces.m_global_vars_base->m_interval_per_tick );
	//
	// const auto eye_position     = g_ctx.m_local->get_eye_position( false );
	// int best_tick               = g_ctx.m_cmd->m_tick_count;
	// static int best_logged_tick = 0;
	//
	// g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
	//	if ( !entity || !entity->is_alive( ) || entity->is_dormant( ) || !g_ctx.m_local->is_enemy( entity ) || entity == g_ctx.m_local ) {
	//		angles_to_head = { };
	//		current_fov    = 180.f;
	//		return;
	//	}
	//
	//	const auto index = entity->get_index( );
	//
	//	if ( const auto record_list = g_lagcomp.m_records[ index ] ) {
	//		for ( int i = 0; i < max_allocation; i++ ) {
	//			auto record = record_list[ i ];
	//
	//			if ( !record.m_valid )
	//				continue;
	//
	//			for ( int j = 0; j < 19; j++ ) {
	//				angles_to_head = g_math.calculate_angle( eye_position, entity->get_hitbox_position( j, record.m_matrix ) );
	//
	//				if ( float calculated_fov = g_math.calculate_fov( g_ctx.m_cmd->m_view_point, angles_to_head ); calculated_fov < current_fov ) {
	//					current_fov = calculated_fov;
	//
	//					best_tick        = static_cast< int >( ( record.m_sim_time + g_lagcomp.lerp_time( ) ) /
	//                                                    g_interfaces.m_global_vars_base->m_interval_per_tick );
	//					best_logged_tick = i;
	//				}
	//			}
	//		}
	//	}
	//} );
	//
	//	if ( !( g_ctx.m_cmd->m_buttons & in_attack ) )
	//	return;
	//
	// g_ctx.m_cmd->m_tick_count = best_tick;
	//
	// auto recoil_angle = g_ctx.m_local->get_punch( ) * ( g_convars[ HASH_BT( "weapon_recoil_scale" ) ]->get_float( ) * -1.f );
	//
	// angles_to_head += recoil_angle;
	//
	// g_ctx.m_cmd->m_view_point = angles_to_head.normalize( );
	//
	// g_interfaces.m_engine_client->set_view_angles( g_ctx.m_cmd->m_view_point );
}
