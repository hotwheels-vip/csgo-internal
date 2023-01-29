#include "misc.h"
#include "../../logging/logging.h"
#include "../../source_engine/enumerations/e_flags.h"
#include "../../source_engine/enumerations/e_item_definition_index.h"

void misc_t::on_paint_traverse( )
{
	if ( !render.m_initialised )
		return;
}

void misc_t::on_end_scene( )
{
	[ & ]( const bool can_draw_watermark ) {
		if ( !can_draw_watermark )
			return;

		const ImColor white        = ImColor( 1.f, 1.f, 1.f, 1.f );
		const ImColor accent_color = ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent );
		const ImColor grey         = ImGui::GetColorU32( ImGuiCol_::ImGuiCol_TextDisabled );
		const ImColor black        = ImColor( 0.f, 0.f, 0.f, 1.f );

		static float frame_rate_f = 1.0f;
		frame_rate_f              = ( 0.9f * frame_rate_f ) + ( 0.1f * memory.m_globals->m_abs_frame_time ); /* TODO ~ store this globally */

		static auto width = 0.f;

		ImGui::SetNextWindowSize( ImVec2( -1, 29.f ), ImGuiCond_::ImGuiCond_Always );
		ImGui::SetNextWindowPos( ImVec2( globals.m_display_size.x - width - 5.f, 5.f ), ImGuiCond_::ImGuiCond_Always );
		ImGui::Begin( ( "hotwheels-practice-window-ui" ), 0,
		              ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		                  ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize );
		{
			const auto window = ImGui::GetCurrentWindow( );

			const auto draw_list = window->DrawList;

			const auto size     = window->Size;
			const auto position = window->Pos;

			/* render outline */
			ImGui::PushClipRect( ImVec2( position.x + 1.f, position.y + 1.f ), ImVec2( position.x + size.x - 1.f, position.y + size.y - 1.f ),
			                     false );
			draw_list->AddRect( ImVec2( position.x + 1.f, position.y + 1.f ), ImVec2( position.x + size.x - 1.f, position.y + size.y - 1.f ),
			                    ImColor( 50 / 255.f, 50 / 255.f, 50 / 255.f ), ImGui::GetStyle( ).WindowRounding );
			ImGui::PopClipRect( );

			ImGui::PushFont( render.m_fonts[ e_font_names::font_name_verdana_bd_11 ] );

			ImGui::Text( "hotwheels" );
			ImGui::SameLine( 0.f, 0.f );
			ImGui::TextColored( accent_color, ".vip" );

			ImGui::SameLine( );

			ImGui::TextColored( ImVec4( 1.f, 1.f, 1.f, 0.1f ), "|" ); /* SORRY BOUT dat.. */

			ImGui::SameLine( );

			ImGui::Text( std::to_string( frame_rate_f != 0.0f ? static_cast< int >( 1 / frame_rate_f ) : 0 ).c_str( ) );
			
			ImGui::SameLine( );

			ImGui::TextColored( grey, "fps" );

			ImGui::PopFont( );

			width = ImGui::GetWindowSize( ).x;
		}
		ImGui::End( );
	}( GET_CONFIG_BOOL( variables.m_menu.m_watermark ) );

	// functions that require player to be ingame
	if ( !globals.m_local || !interfaces.m_engine->is_in_game( ) || globals.m_local->get_observer_mode( ) == e_obs_mode::obs_mode_deathcam )
		return;

	// practice window
	[ & ]( const bool can_draw_practice_window ) {
		// TODO: add a is in valve server check(aka in mm game check) and dont even display anything

		if ( !can_draw_practice_window )
			return;

		if ( !globals.m_local->is_alive( ) )
			return;

		const ImColor accent_color = ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent );

		constexpr auto background_height = 25.f;
		constexpr auto title_text        = "practice";
		const auto title_text_size       = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
            render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, title_text );

		ImGui::SetNextWindowSizeConstraints( ImVec2( title_text_size.x + 25.f, title_text_size.y + 5.f ), ImVec2( FLT_MAX, FLT_MAX ) );
		ImGui::Begin( ( "hotwheels-practice-window-ui" ), 0,
		              ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		                  ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize );
		{
			const auto window = ImGui::GetCurrentWindow( );

			const auto draw_list = window->DrawList;

			const auto size     = window->Size;
			const auto position = window->Pos;

			[ & ]( ) {
				/* render background */
				ImGui::PushClipRect( ImVec2( position.x, position.y ), ImVec2( position.x + size.x, position.y + background_height ), false );
				draw_list->AddRectFilled( ImVec2( position.x, position.y ), ImVec2( position.x + size.x, position.y + background_height ),
				                          ImColor( 25 / 255.f, 25 / 255.f, 25 / 255.f, 1.f ), ImGui::GetStyle( ).WindowRounding,
				                          ImDrawFlags_RoundCornersTop );
				ImGui::PopClipRect( );

				/* render gradient */
				RenderFadedGradientLine( draw_list, ImVec2( position.x, position.y + background_height - 1.f ), ImVec2( size.x, 1.f ),
				                         ImColor( accent_color.Value.x, accent_color.Value.y, accent_color.Value.z ) );

				draw_list->AddText(
					render.m_fonts[ e_font_names::font_name_verdana_bd_11 ], render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
					ImVec2( position.x + ( ( size.x - title_text_size.x ) / 2.f ), position.y + ( ( background_height - title_text_size.y ) / 2.f ) ),
					ImColor( 1.f, 1.f, 1.f ), title_text );

				/* render outline */
				ImGui::PushClipRect( ImVec2( position.x + 1.f, position.y + 1.f ), ImVec2( position.x + size.x - 1.f, position.y + size.y - 1.f ),
				                     false );
				draw_list->AddRect( ImVec2( position.x + 1.f, position.y + 1.f ), ImVec2( position.x + size.x - 1.f, position.y + size.y - 1.f ),
				                    ImColor( 50 / 255.f, 50 / 255.f, 50 / 255.f ), ImGui::GetStyle( ).WindowRounding );
				ImGui::PopClipRect( );
			}( );

			ImGui::SetCursorPosY( 30.f );

			if ( !convars.find( fnv1a::hash_const( "sv_cheats" ) )->get_bool( ) ) {
				ImGui::Text( "sv_cheats 1 is not enabled" );
			} else {
				const auto cp_key = GET_CONFIG_BIND( variables.m_misc.m_practice_cp_key ).m_key;

				ImGui::Text( std::format( "{} - checkpoint", cp_key != 0 ? filtered_key_names[ cp_key ] : "none" ).c_str( ) );

				const auto tp_key = GET_CONFIG_BIND( variables.m_misc.m_practice_tp_key ).m_key;

				ImGui::Text( std::format( "{} - teleport", tp_key != 0 ? filtered_key_names[ tp_key ] : "none" ).c_str( ) );
			}
		}
		ImGui::End( );
	}( GET_CONFIG_BOOL( variables.m_misc.m_practice_window ) );
}

void misc_t::on_create_move_pre( )
{
	[ & ]( const bool can_run_practice_window ) {
		if ( !can_run_practice_window )
			return;

		if ( !convars.find( fnv1a::hash_const( "sv_cheats" ) )->get_bool( ) )
			return;

		const auto cp_key = GET_CONFIG_BIND( variables.m_misc.m_practice_cp_key ).m_key;

		const auto tp_key = GET_CONFIG_BIND( variables.m_misc.m_practice_tp_key ).m_key;

		if ( input.is_key_released( cp_key ) ) {
			if ( !( globals.m_local->flags( ) & fl_onground ) ) {
				g_log.print( "you need to be onground to set a checkpoint.", "[practice]" );
				return;
			}

			c_angle saved_angles = { };
			interfaces.m_engine->get_view_angles( saved_angles );

			practice.saved_angles   = saved_angles;
			practice.saved_position = globals.m_local->abs_origin( );

			g_log.print( "saved checkpoint.", "[practice]" );
		} else if ( input.is_key_released( tp_key ) ) {
			if ( practice.saved_angles.is_zero( ) || practice.saved_position.is_zero( ) )
				return;

			interfaces.m_engine->client_cmd_unrestricted(
				std::string( "setpos " )
					.append( std::vformat(
						"{} {} {}", std::make_format_args( practice.saved_position.m_x, practice.saved_position.m_y, practice.saved_position.m_z ) ) )
					.append( ";setang " )
					.append( std::vformat(
						"{} {} {}", std::make_format_args( practice.saved_angles.m_x, practice.saved_angles.m_y, practice.saved_angles.m_z ) ) )
					.c_str( ) );

			g_log.print( "teleported to saved checkpoint.", "[practice]" );
		}
	}( GET_CONFIG_BOOL( variables.m_misc.m_practice_window ) );
}
