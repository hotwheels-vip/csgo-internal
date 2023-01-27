#include "world.h"
#include "../../../includes.h"

void world_t::on_frame_stage_notify( e_client_frame_stage stage )
{
	//const int max_index = interfaces.m_client_entity_list->get_highest_entity_index( );

	//if ( stage == e_client_frame_stage::start ) {
	//	for ( int i = 0; i < max_index; i++ /* NOTE ~ i don't like this, i will add an edict cache as well as player cache. . this is sus.. sy. */ ) {
	//		const auto entity = reinterpret_cast< c_fog_controller* >( interfaces.m_client_entity_list->get_client_entity( i ) );

	//		if ( !entity || entity->is_player( ) || entity->is_dormant( ) || entity->client_class()->m_class_id != e_class_index::cfogcontroller )
	//			continue;

	//		/* TODO ~ check if Shutdown() is being called, if so, disable fog */
	//	}
	//}
}
