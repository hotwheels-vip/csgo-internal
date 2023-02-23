#include "../../game/sdk/includes/includes.h"
#include "../../globals/logger/logger.h"
#include "../hooks.h"

bool __fastcall n_detoured_functions::fire_event_intern( void* ecx, void* edx, game_event_t* game_event )
{
	static auto original = g_hooks.m_fire_event_intern.get_original< decltype( &n_detoured_functions::fire_event_intern ) >( );

	const auto hashed_event = HASH_RT( game_event->get_name( ) );

	if ( !g_render.m_initialised )
		return original( ecx, edx, game_event );

	const ImColor im_accent_color = ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent );
	const c_unsigned_char_color accent_color =
		c_unsigned_char_color( im_accent_color.Value.x * 255, im_accent_color.Value.y * 255, im_accent_color.Value.z * 255, 255 );

	// TODO: move this to a separate function in hacks/events/events.cpp

	[ & ]( ) {
		int m_attacker{ }, m_victim{ }, m_group{ }, m_health{ }, m_damage{ };
		std::string m_name{ };

		switch ( hashed_event ) {
		case HASH_BT( "player_hurt" ):
			m_attacker = g_interfaces.m_engine_client->get_player_for_user_id( game_event->get_int( "attacker" ) );
			m_victim   = g_interfaces.m_engine_client->get_player_for_user_id( game_event->get_int( "userid" ) );

			if ( m_attacker < 1 || m_attacker > 64 || m_victim < 1 || m_victim > 64 )
				return;
			else if ( m_attacker != g_interfaces.m_engine_client->get_local_player( ) ||
			          m_victim == g_interfaces.m_engine_client->get_local_player( ) )
				return;

			m_group = game_event->get_int( "hitgroup" );

			if ( m_group == hitgroup_gear )
				return;

			auto m_target = g_interfaces.m_client_entity_list->get< c_base_entity* >( m_victim );
			if ( !m_target )
				return;

			player_info_t info;
			if ( !g_interfaces.m_engine_client->get_player_info( m_victim, &info ) )
				return;
			m_name   = std::string( info.m_name ).substr( 0, 24 );
			m_damage = game_event->get_int( "dmg_health" );
			m_health = game_event->get_int( "health" );

			if ( g_config.get< std::vector< bool > >( g_variables.m_log_types )[ e_log_types::log_type_hit_enemy ] ) {
				std::string out =
					std::vformat( "hit {} for {} in the {}\n", std::make_format_args( m_name, m_damage, g_utilities.m_hit_groups[ m_group ] ) );

				g_logger.print( out, "[damage]" );

				g_interfaces.m_convar->console_color_printf( accent_color, "[damage] " );
				g_interfaces.m_convar->console_color_printf( c_unsigned_char_color::console_text_color( ), out.c_str( ) );
			}
			break;
		}
	}( );

	return original( ecx, edx, game_event );
}
