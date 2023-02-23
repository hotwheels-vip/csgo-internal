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

	if ( !( g_ctx.m_cmd->m_buttons & in_attack ) )
		return;

	auto entity = find_closest_player( );

	if ( !entity )
		return;

	const auto eye_position = g_ctx.m_local->get_eye_position( false );

	// TODO: autowalls
	if ( !g_ctx.m_local->can_see_player( entity ) )
		return;

	if ( GET_VARIABLE( g_variables.m_backtrack_enable, bool ) )
		g_lagcomp.backtrack_player( entity );
	else // sanity
		g_ctx.m_record = nullptr;

	c_angle angles_to_head{ };

	// if valid record, recalculate to matrix hitbox pos
	if ( g_ctx.m_record ) {
		angles_to_head = g_math.calculate_angle( eye_position, entity->get_hitbox_position( hitbox_head, g_ctx.m_record->m_matrix ) );
	} else
		angles_to_head = g_math.calculate_angle( eye_position, entity->get_hitbox_position( hitbox_head ) );

	auto recoil_angle = g_ctx.m_local->get_punch( ) * ( g_convars[ HASH_BT( "weapon_recoil_scale" ) ]->get_float( ) * -1.f );

	angles_to_head += recoil_angle;

	angles_to_head = angles_to_head.normalize( );

	g_interfaces.m_engine_client->set_view_angles( angles_to_head );

	g_ctx.m_cmd->m_view_point = angles_to_head;
}
