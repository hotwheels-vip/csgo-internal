#include "utilities.h"

#include "../game/sdk/includes/includes.h"
#include "../globals/includes/includes.h"

/* used ~ std::thread (*/
#include <thread>

int n_utilities::impl_t::create_thread( unsigned long __stdcall function( void* ), void* parameter )
{
	/* void* handle = LI_FN( CreateThread )( nullptr, 0, function, parameter, 0, nullptr );
	if ( !handle )
	    return 0;

	LI_FN( CloseHandle )( handle ); */

	std::thread( function, parameter ).detach( );

	return 1;
}

bool n_utilities::impl_t::is_weapon_valid( )
{
	auto weapon_handle = g_ctx.m_local->get_active_weapon_handle( );
	if ( !weapon_handle )
		return false;

	const auto active_weapon =
		reinterpret_cast< c_base_entity* >( g_interfaces.m_client_entity_list->get_client_entity_from_handle( weapon_handle ) );

	if ( !active_weapon )
		return false;

	const auto definition_index = active_weapon->get_item_definition_index( );

	return definition_index > 0 && !( definition_index >= 41 && definition_index <= 59 ) && !( definition_index >= 68 );
}
