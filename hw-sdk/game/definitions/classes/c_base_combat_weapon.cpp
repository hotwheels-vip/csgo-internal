#include "../../sdk/classes/c_base_combat_weapon.h"

#include "../../../globals/includes/includes.h"
#include "../../sdk/classes/c_cs_player.h"

bool sdk::c_base_combat_weapon::can_shoot_primary( )
{
	auto entity = g_interfaces.entity_list->get_client_entity_from_handle< sdk::c_cs_player* >( owner_entity( ) );

	if ( !entity )
		return false;

	auto server_time = entity->tick_base( ) * g_interfaces.globals->interval_per_tick;

	if ( server_time >= next_primary_attack( ) )
		return true;

	return false;
}

bool sdk::c_base_combat_weapon::can_shoot_secondary( )
{
	auto entity = g_interfaces.entity_list->get_client_entity_from_handle< sdk::c_cs_player* >( owner_entity( ) );

	if ( !entity )
		return false;

	auto server_time = entity->tick_base( ) * g_interfaces.globals->interval_per_tick;

	if ( server_time >= next_secondary_attack( ) )
		return true;

	return false;
}
