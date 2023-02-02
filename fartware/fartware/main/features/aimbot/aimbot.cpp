#include "aimbot.h"
#include "../lagcomp/lagcomp.h"

#include "../../convars/convars.h"

#include "../../globals.h"

#include "../../source_engine/enumerations/e_item_definition_index.h"
#include "../../source_engine/interfaces/interfaces.h"

#include "../../utilities/console/console.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../../utilities/utilities.h"

#include "../../config/variables.h"
#include "../entities/entities.h"

c_base_entity* aimbot_t::find_closest( c_cconvar* recoil_scale )
{
	float closest_fov             = std::numeric_limits< float >::max( );
	c_base_entity* closest_player = nullptr;
	const auto eye_position       = globals.m_local->eye_position( );

	entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
		if ( !entity->is_valid( ) ) /* TODO ~ add entity->is_valid() check with params (check for team, dormant etc) */
			return;

		c_vector head_position = entity->get_bone_position( e_hitgroup::hitgroup_head );

		c_angle delta = mathematics.calculate_angle( eye_position, head_position );
		delta -= ( globals.m_local->aim_punch_angle( ) * recoil_scale->get_float( ) ); /* account for recoil */

		const float fov = mathematics.calculate_fov( globals.m_cmd->m_view_point, delta );

		if ( fov < closest_fov ) {
			closest_fov    = fov;
			closest_player = entity;
		}
	} );

	return closest_player;
}

bool aimbot_t::is_weapon_valid( c_base_entity* active_weapon )
{
	if ( !active_weapon )
		return false;

	auto definition_index = active_weapon->item_definition_index( );

	// fuck you im not checking these manually

	const bool misc = definition_index >= 41 && definition_index <= 59;

	return definition_index > 0 && !misc && !( definition_index >= 68 );
}

void aimbot_t::on_create_move_post( )
{
	static auto recoil_scale = convars.find( fnv1a::hash_const( "weapon_recoil_scale" ) );

	// TODO add global weapon pointer
	auto weapon_handle = globals.m_local->active_weapon_handle( );
	if ( !weapon_handle )
		return;

	const auto active_weapon = reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity_from_handle( weapon_handle ) );

	if ( !active_weapon )
		return;

	if ( !( globals.m_cmd->m_buttons & e_buttons::in_attack ) )
		return;

	if ( !is_weapon_valid( active_weapon ) )
		return;

	auto entity = find_closest( recoil_scale );

	if ( !entity )
		return;

	const auto head_position = entity->get_bone_position( hitgroup_head );
	const auto local_eye_pos = globals.m_local->eye_position( );

	// TODO autowalls
	if ( !globals.m_local->is_visible( entity, head_position ) )
		return;

	if ( GET_CONFIG_BOOL( variables.m_aimbot.m_backtrack_enabled ) )
		lagcomp.backtrack_player( entity );

	c_vector forward_to_head = local_eye_pos - head_position;
	c_angle angles_to_head{ };

	mathematics.vector_angles( forward_to_head, angles_to_head );

	// if we have a valid record, calculate to it instead
	// this can be improved, but not really quite sure how
	if ( globals.m_record ) {
		forward_to_head = local_eye_pos - entity->get_bone_position( hitgroup_head, globals.m_record->bone_matrix );
		mathematics.vector_angles( forward_to_head, angles_to_head );
	}

	angles_to_head += globals.m_local->aim_punch_angle( ) * ( recoil_scale->get_float( ) * -1.f );

	angles_to_head = angles_to_head.normalize( );

	// if ( active_weapon->can_shoot_primary( ) ) {
	interfaces.m_engine->set_view_angles( angles_to_head );

	globals.m_cmd->m_view_point = angles_to_head;
	//}
}
