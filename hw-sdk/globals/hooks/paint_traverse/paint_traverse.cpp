#include "paint_traverse.h"
#include "../../../hacks/features/visuals/visuals.h"
#include "../../../hacks/logging/logging.h"
#include "../../../hacks/menu/menu.h"
#include "../../ctx/ctx.h"

void __fastcall hooks::paint_traverse::paint_traverse_detour( sdk::i_panel* self, void* edx, unsigned int panel, bool force_repaint,
                                                              bool allow_force )
{
	const auto panel_hash = HASH_RT( g_interfaces.panel->panel_name( panel ) );

	if ( static bool already_logged = false; !already_logged ) {
		//	g_interfaces.engine->execute_client_cmd( _( "clear" ) );
		g_log.print( _( " injection success." ), _( "[hotwheels.vip]" ) );

		already_logged = true;
	}

	if ( panel_hash == HASH_BT( "FocusOverlayPanel" ) ) {
		g_ctx.view_matrix = g_interfaces.engine->world_to_screen_matrix( );

		for ( auto& player_info : g_entity_list.players ) {
			auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( player_info.m_index );

			if ( ( !player_info.m_valid && !player_info.m_dormant_info.m_valid ) || !player )
				continue;

			visuals::esp_object& object = g_visuals.esp_objects[ player->entity_index( ) ];

			if ( object.m_owner != player )
				continue;

			math::vec3 old_origin = player->get_abs_origin( );

			if ( !player_info.m_valid && sdk::ticks_to_time( g_interfaces.globals->tick_count - player_info.m_dormant_info.m_found_tick ) < 3.f ) {
				player->set_abs_origin( sdk::ticks_to_time( g_interfaces.globals->tick_count - player_info.m_dormant_info.m_vouchable_tick ) < 3.f
				                            ? player_info.m_dormant_info.m_vouchable_position
				                            : player_info.m_dormant_info.m_last_position );
				player->invalidate_bone_cache( );
			}

			auto collideable = object.m_owner->get_collideable( );

			if ( !collideable )
				continue;

			player_info.m_mins = collideable->obb_mins( );
			player_info.m_maxs = collideable->obb_maxs( );

			player_info.m_rgfl = player->rgfl_coordinate_frame( );

			if ( !player_info.m_valid && sdk::ticks_to_time( g_interfaces.globals->tick_count - player_info.m_dormant_info.m_found_tick ) < 3.f ) {
				player->set_abs_origin( old_origin );
				player->invalidate_bone_cache( );
			}
		}

		if ( g_menu.menu_open ) {
			g_interfaces.panel->set_input_keyboard_state( panel, true );
			g_interfaces.panel->set_input_mouse_state( panel, true );
		} else {
			g_interfaces.panel->set_input_keyboard_state( panel, false );
			g_interfaces.panel->set_input_mouse_state( panel, false );
		}
	}

	hooks::paint_traverse_hook.call_original< void >( self, edx, panel, force_repaint, allow_force );
}
