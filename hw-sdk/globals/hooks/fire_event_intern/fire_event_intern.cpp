#include "fire_event_intern.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../../../utils/keybinds/keybinds.h"
#include "../../../utils/particle_system/particle_system.h"

bool __fastcall hooks::fire_event_intern::fire_event_intern_detour( void* ecx, void* edx, sdk::i_game_event* game_event, bool server_only,
                                                                    bool client_only )
{
	auto event_name = game_event->get_name( );

	// YIPIIIEEEE HEADSHOT!!!
	// if ( HASH( event_name ) == HASH_BT( "player_death" ) ) {
	//	if ( game_event->get_bool( _( "headshot" ) ) ) {
	//		auto player_id = game_event->get_int( _( "userid" ) );
	//		auto player =
	//			g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( g_interfaces.engine->get_player_for_user_id( player_id ) );
	//
	//		if ( player )
	//			g_particle_system.run( player->hitbox_position( sdk::HITGROUP_HEAD ), _( "weapon_confetti_omni" ) );
	//	}
	//}

	return fire_event_intern_hook.call_original< bool >( ecx, edx, game_event, server_only, client_only );
}
