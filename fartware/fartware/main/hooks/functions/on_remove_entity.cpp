#include "../hooks.h"

#include "../../features/entities/entities.h"

void __fastcall n_detoured_functions::on_remove_entity( int ecx, int edx, void* handle_entity, unsigned int entity_handle )
{
	static auto original = hooks.on_remove_entity.get_original< decltype( &n_detoured_functions::on_remove_entity ) >( );

	entity_cache.on_remove_entity( reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity_from_handle( entity_handle ) ) );

	original( ecx, edx, handle_entity, entity_handle );
}