#include "../../features/entities/avatars.h"
#include "../../features/movement/movement.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::frame_stage_notify( void* thisptr, int edx, e_client_frame_stage stage )
{
	static auto original = hooks.frame_stage_notify.get_original< decltype( &n_detoured_functions::frame_stage_notify ) >( );

	if ( !interfaces.m_engine->is_in_game( ) )
		return original( thisptr, edx, stage );

	globals.m_local =
		reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity( interfaces.m_engine->get_local_player( ) ) );

	if ( !globals.m_local )
		return original( thisptr, edx, stage );

	switch ( stage ) {
	case e_client_frame_stage::start:
		globals.m_view_matrix = interfaces.m_engine->world_to_screen_matrix( );

		avatar_cache.on_frame_stage_notify( );

		movement.handle_edgebug_view_point( );
		break;
	}

	original( thisptr, edx, stage );
}
