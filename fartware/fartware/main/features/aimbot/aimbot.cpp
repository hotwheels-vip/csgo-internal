#include "aimbot.h"
#include "../../convars/convars.h"
#include "../../globals.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../entities/entities.h"

/* AM VERY SORRY */
void aimbot_t::on_create_move_post( )
{
	if ( !( globals.m_cmd->m_buttons & e_buttons::in_attack ) )
		return;

	float field_of_view     = 180.f;
	const auto eye_position = globals.m_local->eye_position( );

	constexpr auto get_bone_index = [ & ]( c_base_entity* entity, const char* bone_name ) { return entity->lookup_bone( bone_name ); };

	const auto get_best_entity = [ & ]( ) {
		c_base_entity* best_entity = nullptr;

		entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
			if ( !entity || !entity->is_alive( ) || entity->is_dormant( ) || entity->team( ) == globals.m_local->team( ) ||
			     entity == globals.m_local ) /* TODO ~ add entity->is_valid() check with params (check for team, dormant etc) */
				return;

			if ( int head_bone_number = get_bone_index( entity, "head_0" ); head_bone_number != -1 ) {
				c_vector head_position{ };
				entity->get_bone_position( head_bone_number, head_position );

				c_angle delta = mathematics.calculate_angle( eye_position, head_position );
				delta -= ( globals.m_local->aim_punch_angle( ) *
				           convars.find( fnv1a::hash_const( "weapon_recoil_scale" ) )->get_float( ) ); /* account for recoil */

				if ( const float fov = mathematics.calculate_fov( globals.m_cmd->m_view_point, delta ); fov < field_of_view ) {
					field_of_view = fov;
					best_entity   = entity;
				}
			}
		} );

		return best_entity;
	};

	const auto best_entity = get_best_entity( );

	if ( !best_entity )
		return;

	if ( int head_bone_number = get_bone_index( best_entity, "head_0" ); head_bone_number != -1 ) {
		/*if ( auto active_weapon = reinterpret_cast< c_base_entity* >(
				 interfaces.m_client_entity_list->get_client_entity_from_handle( globals.m_local->active_weapon_handle( ) ) );
		     active_weapon && active_weapon->can_shoot( ) ) {*/

			c_vector head_position{ };
			best_entity->get_bone_position( head_bone_number, head_position );

			c_angle delta = mathematics.calculate_angle( eye_position, head_position ) - globals.m_cmd->m_view_point;

			delta -= ( globals.m_local->aim_punch_angle( ) *
			           convars.find( fnv1a::hash_const( "weapon_recoil_scale" ) )->get_float( ) ); /* account for recoil */

			delta.normalize( );

			globals.m_cmd->m_view_point += delta;
			interfaces.m_engine->set_view_angles( globals.m_cmd->m_view_point );
		/*}*/
	}
}