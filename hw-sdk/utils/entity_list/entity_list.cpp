#include "entity_list.h"
#include "../../game/sdk/classes/c_base_entity.h"
#include "../../globals/ctx/ctx.h"

void entity_list::impl::update( )
{
	g_ctx.local  = nullptr;
	g_ctx.weapon = nullptr;

	auto local_player_entity = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( g_interfaces.engine->get_local_player( ) );

	if ( !local_player_entity )
		return;

	g_ctx.local = local_player_entity;

	if ( auto local_weapon_handle = g_ctx.local->active_weapon( ) ) {
		auto local_weapon_entity = g_interfaces.entity_list->get_client_entity_from_handle< sdk::c_base_combat_weapon* >( local_weapon_handle );

		if ( local_weapon_entity )
			g_ctx.weapon = local_weapon_entity;
	}

	for ( int iterator = 0; iterator < 65; iterator++ ) {
		auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( iterator );

		auto& player_info = players[ iterator ];

		player_info.m_valid = false;

		if ( !player || player == g_ctx.local )
			continue;

		if ( player->is_player( ) ) {
			player_info.m_dormant_info.m_valid = false;

			if ( !player->is_alive( ) )
				continue;

			if ( player->is_dormant( ) ) {
				if ( sdk::ticks_to_time( g_interfaces.globals->tick_count - player_info.m_dormant_info.m_found_tick ) < 3.f )
					player_info.m_dormant_info.m_valid = true;

				continue;
			}

			if ( !player->is_enemy( g_ctx.local ) )
				continue;

			player_info.m_valid = true;
			player_info.m_name  = player->name( );
			player_info.m_index = iterator;

			player_info.m_dormant_info.m_last_position = player->get_abs_origin( );
			player_info.m_dormant_info.m_found_tick    = g_interfaces.globals->tick_count;

			player_info.m_dormant_info.m_vouchable_position = player->get_abs_origin( );
			player_info.m_dormant_info.m_vouchable_tick     = g_interfaces.globals->tick_count;

			if ( auto weapon_handle = player->active_weapon( ) ) {
				auto weapon_entity = g_interfaces.entity_list->get_client_entity_from_handle< sdk::c_base_combat_weapon* >( weapon_handle );

				if ( weapon_entity )
					player_info.m_weapon = weapon_entity;
			}
		}
	}
}
