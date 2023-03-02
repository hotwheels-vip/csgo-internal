#include "../../game/sdk/includes/includes.h"
#include "../../globals/logger/logger.h"
#include "../../hacks/misc/scaleform/scaleform.h"
#include "../hooks.h"
#include "../../hacks/visuals/players/dormancy/dormancy.h"
#include "../../hacks/visuals/players/players.h"

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
		c_base_entity* m_attacker_ent{ };

		switch ( hashed_event ) {
		case HASH_BT( "game_newmawp" ):
			g_scaleform.m_should_force_update = true;
			for ( int i = 0; i < 64; i++ ) {
				g_players.m_stored_cur_time[ i ] = 0.f;
				g_players.m_fading_alpha[ i ] = 0.f;
				g_dormancy.m_sound_players[ i ].reset( );
			}
			break;
		case HASH_BT( "bot_takeover" ):
		case HASH_BT( "switch_team" ):
		case HASH_BT( "round_start" ):
			g_scaleform.m_should_update_teamcount_avatar = true;
			for ( int i = 0; i < 64; i++ ) {
				g_players.m_stored_cur_time[ i ] = 0.f;
				g_players.m_fading_alpha[ i ]    = 0.f;
				g_dormancy.m_sound_players[ i ].reset( );
			}
			break;
		case HASH_BT( "round_end" ):
			g_scaleform.m_should_update_winpanel = true;
			break;
		case HASH_BT( "player_death" ):
			g_scaleform.m_should_update_deathnotices = true;
			break;
		case HASH_BT( "player_hurt" ):
			m_attacker     = g_interfaces.m_engine_client->get_player_for_user_id( game_event->get_int( "attacker" ) );
			m_victim       = g_interfaces.m_engine_client->get_player_for_user_id( game_event->get_int( "userid" ) );
			m_attacker_ent = g_interfaces.m_client_entity_list->get< c_base_entity >( m_attacker );

			if ( m_attacker < 1 || m_attacker > 64 || m_victim < 1 || m_victim > 64 )
				break;
			else if ( m_attacker != g_interfaces.m_engine_client->get_local_player( ) ||
			          m_victim == g_interfaces.m_engine_client->get_local_player( ) )
				break;

			m_group = game_event->get_int( "hitgroup" );

			if ( m_group == hitgroup_gear )
				break;

			auto m_target = g_interfaces.m_client_entity_list->get< c_base_entity* >( m_victim );
			if ( !m_target )
				break;

			player_info_t info;
			if ( !g_interfaces.m_engine_client->get_player_info( m_victim, &info ) )
				break;
			m_name   = std::string( info.m_name ).substr( 0, 24 );
			m_damage = game_event->get_int( "dmg_health" );
			m_health = game_event->get_int( "health" );

			if ( g_config.get< std::vector< bool > >( g_variables.m_log_types )[ e_log_types::log_type_hit_enemy ] ) {
				g_logger.print(
					std::vformat( "hit {} for {} in the {}\n", std::make_format_args( m_name, m_damage, g_utilities.m_hit_groups[ m_group ] ) ),
					"[damage]" );

				const std::string out =
					std::vformat( "hit {} | dealt: {}hp | hitgroup: {} | {} health remaining | backtrack: {} ticks\n",
				                  std::make_format_args( m_name, m_damage, g_utilities.m_hit_groups[ m_group ], m_health,
				                                         g_ctx.m_record ? g_math.time_to_ticks( std::fabsf( m_attacker_ent->get_simulation_time( ) -
				                                                                                            g_ctx.m_record->m_sim_time ) )
				                                                        : 0 ) );

				g_interfaces.m_convar->console_color_printf( accent_color, "[damage] " );
				g_interfaces.m_convar->console_color_printf( c_unsigned_char_color::console_text_color( ), out.c_str( ) );
			}
			break;
		}
	}( );

	return original( ecx, edx, game_event );
}
