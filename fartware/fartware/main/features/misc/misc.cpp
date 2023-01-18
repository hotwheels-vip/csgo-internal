#include "misc.h"
#include "../../logging/logging.h"
#include "../../source_engine/enumerations/e_flags.h"

void misc_t::practice_t::think( )
{
	if ( !GET_CONFIG_BOOL( variables.m_misc.m_practice_window ) )
		return;

	const auto sv_cheats = convars.find( fnv1a::hash_const( "sv_cheats" ) )->get_bool( );

	if ( !globals.m_local || interfaces.m_engine->is_in_game( ) || !sv_cheats )
		return;

	if ( input.is_key_released( GET_CONFIG_BIND( variables.m_misc.m_practice_cp_key ).m_key ) ) {
		if ( !( globals.m_local->flags( ) & fl_onground ) ) {
			g_log.print( "you have to be on ground to set a checkpoint.", "[practice]" );
			return;
		}

		misc.practice.pos = globals.m_local->abs_origin( );
		interfaces.m_engine->get_view_angles( misc.practice.ang );

		g_log.print( "checkpoint set.", "[practice]" );
	} else if ( input.is_key_released( GET_CONFIG_BIND( variables.m_misc.m_practice_tp_key ).m_key ) ) {
		if ( misc.practice.pos.is_zero( ) || misc.practice.ang.is_zero( ) )
			return;
		interfaces.m_engine->client_cmd_unrestricted(
			std::string( "setpos " )
				.append( std::vformat( "{} {} {}", std::make_format_args( misc.practice.pos.m_x, misc.practice.pos.m_y, misc.practice.pos.m_z ) ) )
				.append( ";setang " )
				.append( std::vformat( "{} {} {}", std::make_format_args( misc.practice.ang.m_x, misc.practice.ang.m_y, misc.practice.ang.m_z ) ) )
				.c_str( ) );
	}
}

void misc_t::practice_t::render( )
{
	if ( !GET_CONFIG_BOOL( variables.m_misc.m_practice_window ) )
		return;

	const auto sv_cheats = convars.find( fnv1a::hash_const( "sv_cheats" ) )->get_bool( );

	if ( !globals.m_local || interfaces.m_engine->is_in_game( ) || !sv_cheats )
		return;

	ImGui::Begin( "hotwheels-practice-window", nullptr,
	              ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
	                  ImGuiWindowFlags_AlwaysAutoResize );

	ImDrawList* draw_list = ImGui::GetWindowDrawList( );

	ImGui::SetCursorPos( ImVec2( ImGui::GetWindowSize( ).x / 2 - ImGui::CalcTextSize( "practice" ).x / 2, 20 / 2 - 5 ) );
	ImGui::TextColored( ImVec4( Accent[ 0 ], Accent[ 1 ], Accent[ 2 ], 1.f ), "practice" );

	ImGui::Text( "%s - checkpoint", GET_CONFIG_BIND( variables.m_misc.m_practice_cp_key ).m_key != 0
	                                    ? filtered_key_names[ GET_CONFIG_BIND( variables.m_misc.m_practice_cp_key ).m_key ]
	                                    : "none" );
	ImGui::Text( "%s - teleport", GET_CONFIG_BIND( variables.m_misc.m_practice_tp_key ).m_key != 0
	                                  ? filtered_key_names[ GET_CONFIG_BIND( variables.m_misc.m_practice_tp_key ).m_key ]
	                                  : "none" );

	ImGui::End( );
}
