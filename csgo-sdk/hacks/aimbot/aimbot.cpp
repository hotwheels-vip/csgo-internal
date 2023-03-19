#include "aimbot.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../auto_wall/auto_wall.h"
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

		const auto hitbox_position = entity->get_hitbox_position( hitbox_head );

		auto angle = g_math.calculate_angle( eye_position, hitbox_position );
		angle -= ( g_ctx.m_local->get_punch( ) * g_convars[ HASH_BT( "weapon_recoil_scale" ) ]->get_float( ) );

		if ( GET_VARIABLE( g_variables.m_backtrack_enable, bool ) ) {
			g_lagcomp.backtrack_player( entity );
			if ( !g_ctx.m_local->can_see_player( entity ) && !g_ctx.m_local->can_see_matrix( entity, g_ctx.m_record->m_matrix ) )
				return;
		} else {
			g_ctx.m_record = nullptr;

			fire_bullet_data_t data = { };
			if ( g_auto_wall.get_damage( hitbox_position, &data ) < 0.f )
				return;
		}

		if ( float calculated_fov = g_math.calculate_fov( g_prediction.backup_data.m_view_angles, angle ); calculated_fov < closest_fov ) {
			closest_fov    = calculated_fov;
			closest_player = entity;
		}
	} );

	return closest_player;
}

bool n_aimbot::impl_t::can_aimbot( )
{
	return GET_VARIABLE( g_variables.m_aimbot_enable, bool ) && g_ctx.m_cmd->m_tick_count != 0 && g_ctx.m_cmd->m_buttons & in_attack;
}

void n_aimbot::impl_t::on_create_move_post( )
{
	if ( !can_aimbot( ) )
		return;

	auto entity = find_closest_player( );
	if ( !entity )
		return;

	const auto eye_position = g_ctx.m_local->get_eye_position( false );

	const auto weapon = g_interfaces.m_client_entity_list->get< c_base_entity >( g_ctx.m_local->get_active_weapon_handle( ) );
	if ( !weapon )
		return;

	const auto weapon_data = g_interfaces.m_weapon_system->get_weapon_data( weapon->get_item_definition_index( ) );
	if ( !weapon_data )
		return;

	if ( !weapon_data->is_gun( ) )
		return;

	if ( !( g_ctx.m_local->can_shoot( weapon ) ) )
		return;

	c_angle angles_to_head = g_ctx.m_record
	                             ? g_math.calculate_angle( eye_position, entity->get_hitbox_position( hitbox_head, g_ctx.m_record->m_matrix ) )
	                             : g_math.calculate_angle( eye_position, entity->get_hitbox_position( hitbox_head ) );

	angles_to_head -= g_ctx.m_cmd->m_view_point;
	angles_to_head -= ( g_ctx.m_local->get_punch( ) * g_convars[ HASH_BT( "weapon_recoil_scale" ) ]->get_float( ) );
	angles_to_head.normalize( );

	g_ctx.m_cmd->m_view_point += angles_to_head;

	g_interfaces.m_engine_client->set_view_angles( g_ctx.m_cmd->m_view_point );
}
