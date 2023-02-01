#include "aimbot.h"

#include "../../convars/convars.h"

#include "../../globals.h"

#include "../../source_engine/enumerations/e_item_definition_index.h"
#include "../../source_engine/interfaces/interfaces.h"

#include "../../utilities/console/console.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../../utilities/utilities.h"

#include "../../config/variables.h"
#include "../entities/entities.h"

bool aimbot_t::is_weapon_valid( )
{
	auto weapon_handle = globals.m_local->active_weapon_handle( );
	if ( !weapon_handle )
		return false;

	const auto active_weapon = reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity_from_handle( weapon_handle ) );

	if ( !active_weapon )
		return false;

	auto definition_index = active_weapon->item_definition_index( );

	// fuck you im not checking these manually

	const bool misc = definition_index >= 41 && definition_index <= 59;

	return definition_index > 0 && !misc && !( definition_index >= 68 );
}

/* AM VERY SORRY */
void aimbot_t::on_create_move_post( )
{
	if ( !( globals.m_cmd->m_buttons & e_buttons::in_attack ) )
		return;

	if ( !is_weapon_valid( ) )
		return;

	float max_fov = 30.f;

	const auto eye_position = globals.m_local->eye_position( );

	const auto get_best_entity = [ & ]( ) {
		c_base_entity* best_entity = nullptr;

		entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
			if ( !entity || !entity->is_alive( ) || entity->is_dormant( ) || entity->team( ) == globals.m_local->team( ) ||
			     entity == globals.m_local ) /* TODO ~ add entity->is_valid() check with params (check for team, dormant etc) */
				return;

			c_vector head_position = entity->get_bone_position( e_bone_index::bone_head );

			/*if ( !globals.m_local->is_visible( entity, head_position ) )
				return;*/

			c_angle delta = mathematics.calculate_angle( eye_position, head_position );
			delta -= ( globals.m_local->aim_punch_angle( ) *
			           convars.find( fnv1a::hash_const( "weapon_recoil_scale" ) )->get_float( ) ); /* account for recoil */

			const float fov = mathematics.calculate_fov( globals.m_cmd->m_view_point, delta );

			if ( fov < max_fov ) {
				max_fov = fov; 
				best_entity           = entity;
			}
		} );

		return best_entity;
	};

	const auto entity = get_best_entity( );

	if ( !entity )
		return;

	c_vector head_position = entity->get_bone_position( e_bone_index::bone_head );

	c_angle delta = mathematics.calculate_angle( eye_position, head_position ) - globals.m_cmd->m_view_point;

	delta -=
		( globals.m_local->aim_punch_angle( ) * convars.find( fnv1a::hash_const( "weapon_recoil_scale" ) )->get_float( ) ); /* account for recoil */

	delta.normalize( );

	globals.m_cmd->m_view_point += delta;
	interfaces.m_engine->set_view_angles( globals.m_cmd->m_view_point );
}
