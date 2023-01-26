#include "world.h"
#include "../../../includes.h"

void world_t::on_frame_stage( e_client_frame_stage stage )
{
	const int max_index = interfaces.m_client_entity_list->get_highest_entity_index( );

	// this is only here because other features might need to be ran at other frame stages
	if ( stage == e_client_frame_stage::start ) {
		for ( int i = 0; i < max_index; i++ ) {
			auto entity = reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity( i ) );

			if ( !entity || entity->is_player( ) || entity->is_dormant( ) )
				continue;

			// I NEED CLIENT CLASS SHIT TO FINISH THIS. WE DONT EVEN HAVE NETWORK CLASS HOLY FUCKCK

			// auto client_class = entity->get_client_class( );
		}
	}
}
