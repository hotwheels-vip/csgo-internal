#include "misc.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../../globals/logger/logger.h"
#include "../entity_cache/entity_cache.h"

void n_misc::impl_t::on_create_move_pre( )
{
	this->disable_post_processing( );
	this->remove_panorama_blur( );
	this->practice_window_think( );
}

void n_misc::impl_t::on_end_scene( )
{
	[ & ]( const bool can_draw_watermark ) {
		if ( !can_draw_watermark )
			return;

		const auto io     = ImGui::GetIO( );
		static auto width = 0.f;

		ImGui::SetNextWindowSize( ImVec2( -1, 29.f ), ImGuiCond_::ImGuiCond_Always );
		ImGui::SetNextWindowPos( ImVec2( g_ctx.m_width - width - 5.f, 5.f ), ImGuiCond_::ImGuiCond_Always );
		ImGui::Begin( ( "hotwheels-watermark" ), 0,
		              ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		                  ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize |
		                  ImGuiWindowFlags_::ImGuiWindowFlags_NoMove );
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

			ImGui::PushFont( g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ] );

			ImGui::Text( "hotwheels" );
			ImGui::SameLine( 0.f, 0.f );
			ImGui::TextColored( static_cast< ImColor >( ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent ) ), ".vip" );

			ImGui::SameLine( );

			const auto net_channel_info = g_interfaces.m_engine_client->get_net_channel_info( );
			if ( net_channel_info ) {
				ImGui::Text(
					std::to_string( ( std::max )( static_cast< int >( net_channel_info->get_latency( FLOW_OUTGOING ) * 1000.0f ), 0 ) ).c_str( ) );

				ImGui::SameLine( );

				ImGui::TextColored( static_cast< ImColor >( ImGui::GetColorU32( ImGuiCol_::ImGuiCol_TextDisabled ) ), "ms" );

				ImGui::SameLine( );
			}

			ImGui::Text( std::to_string( static_cast< int >( io.Framerate + 0.5f ) ).c_str( ) );

			ImGui::SameLine( );

			ImGui::TextColored( static_cast< ImColor >( ImGui::GetColorU32( ImGuiCol_::ImGuiCol_TextDisabled ) ), "fps" );

			ImGui::PopFont( );

			width = ImGui::GetWindowSize( ).x;
		}
		ImGui::End( );
	}( GET_VARIABLE( g_variables.m_watermark, bool ) );

	// functions that require player to be ingame
	if ( !g_ctx.m_local || !g_interfaces.m_engine_client->is_in_game( ) || g_ctx.m_local->get_observer_mode( ) == 1 /*DEATH_CAM*/ )
		return;

	// practice window
	[ & ]( const bool can_draw_practice_window ) {
		// TODO: add a is in valve server check(aka in mm game check) and dont even display anything

		if ( !can_draw_practice_window )
			return;

		if ( !g_ctx.m_local->is_alive( ) )
			return;

		constexpr auto background_height = 25.f;
		constexpr auto title_text        = "practice";
		const auto title_text_size       = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
				  g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, title_text );

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
				                         static_cast< ImColor >( ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent ) ) );

				draw_list->AddText(
					g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ], g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
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

			if ( !g_convars[ HASH_BT( "sv_cheats" ) ]->get_bool( ) ) {
				ImGui::Text( "sv_cheats is not enabled" );
			} else {
				const auto cp_key = GET_VARIABLE( g_variables.m_practice_cp_key, key_bind_t ).m_key;

				ImGui::Text( std::format( "{} - checkpoint", cp_key != 0 ? FILTERED_KEY_NAMES[ cp_key ] : "none" ).c_str( ) );

				const auto tp_key = GET_VARIABLE( g_variables.m_practice_tp_key, key_bind_t ).m_key;

				ImGui::Text( std::format( "{} - teleport", tp_key != 0 ? FILTERED_KEY_NAMES[ tp_key ] : "none" ).c_str( ) );
			}
		}
		ImGui::End( );
	}( GET_VARIABLE( g_variables.m_practice_window, bool ) );

	[ & ]( bool can_draw_spectator_list ) {
		if ( !can_draw_spectator_list )
			return;

		const ImColor accent_color = ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent );

		std::vector< spectator_data_t > spectator_data{ };

		if ( !g_ctx.m_local || !g_interfaces.m_engine_client->is_in_game( ) || !GET_VARIABLE( g_variables.m_spectators_list, bool ) ) {
			if ( !spectator_data.empty( ) )
				spectator_data.clear( );

			return;
		}

		g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
			if ( !entity || entity->is_alive( ) || entity->is_dormant( ) )
				return;

			const auto entity_index = entity->get_index( );
			const auto entity_team  = entity->get_team( );

			const auto spectated_player = reinterpret_cast< c_base_entity* >(
				g_interfaces.m_client_entity_list->get_client_entity_from_handle( entity->get_observer_target_handle( ) ) );

			if ( !spectated_player || !spectated_player->is_alive( ) )
				return;

			const int spectated_player_index = spectated_player->get_index( );

			player_info_t spectating_info{ }, spectated_info{ };
			g_interfaces.m_engine_client->get_player_info( entity_index, &spectating_info );
			g_interfaces.m_engine_client->get_player_info( spectated_player_index, &spectated_info );

			if ( spectating_info.m_is_hltv )
				return;

			spectator_data.push_back( { std::format( ( "{} -> {}" ), std::string( spectating_info.m_name ).substr( 0, 24 ),
			                                         std::string( spectated_info.m_name ).substr( 0, 24 ) ),
			                            spectated_player == g_ctx.m_local ? GET_VARIABLE( g_variables.m_spectators_list_text_color_one, c_color )
			                                                              : GET_VARIABLE( g_variables.m_spectators_list_text_color_two, c_color ) } );
		} );

		constexpr auto background_height = 25.f;

		constexpr auto title_text  = "spectators";
		const auto title_text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
			g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, title_text );

		ImAnimationHelper spectators_list_ui_animation = ImAnimationHelper( ImHashStr( "hotwheels-spectators-list-ui" ), ImGui::GetIO( ).DeltaTime );
		spectators_list_ui_animation.Update( 2.f, !( spectator_data.empty( ) ) ? 2.f : -2.f );

		// if alpha too low dont run rest of the code
		if ( spectators_list_ui_animation.AnimationData->second < 0.02f )
			return;

		ImGui::PushStyleVar( ImGuiStyleVar_::ImGuiStyleVar_Alpha, spectators_list_ui_animation.AnimationData->second );

		ImGui::SetNextWindowSizeConstraints( ImVec2( title_text_size.x + 20.f, title_text_size.y + 5.f ), ImVec2( FLT_MAX, FLT_MAX ) );
		ImGui::Begin( ( "hotwheels-spectators-list-ui" ), 0,
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
				                          ImColor( 25 / 255.f, 25 / 255.f, 25 / 255.f, spectators_list_ui_animation.AnimationData->second ),
				                          ImGui::GetStyle( ).WindowRounding, ImDrawFlags_RoundCornersTop );
				ImGui::PopClipRect( );

				/* render gradient */
				RenderFadedGradientLine( draw_list, ImVec2( position.x, position.y + background_height - 1.f ), ImVec2( size.x, 1.f ),
				                         ImColor( accent_color.Value.x, accent_color.Value.y, accent_color.Value.z ) );

				draw_list->AddText(
					g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ], g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
					ImVec2( position.x + ( ( size.x - title_text_size.x ) / 2.f ), position.y + ( ( background_height - title_text_size.y ) / 2.f ) ),
					ImColor( 1.f, 1.f, 1.f, spectators_list_ui_animation.AnimationData->second ), title_text );

				/* render outline */
				ImGui::PushClipRect( ImVec2( position.x + 1.f, position.y + 1.f ), ImVec2( position.x + size.x - 1.f, position.y + size.y - 1.f ),
				                     false );
				draw_list->AddRect( ImVec2( position.x + 1.f, position.y + 1.f ), ImVec2( position.x + size.x - 1.f, position.y + size.y - 1.f ),
				                    ImColor( 50 / 255.f, 50 / 255.f, 50 / 255.f, spectators_list_ui_animation.AnimationData->second ),
				                    ImGui::GetStyle( ).WindowRounding );
				ImGui::PopClipRect( );
			}( );

			ImGui::SetCursorPosY( 30.f );

			for ( const auto& data : spectator_data ) {
				// ImGui::Image( data.m_avatar, ImVec2( 14, 14 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ) );
				// ImGui::SameLine( );
				ImGui::TextColored( data.m_color.get_vec4( ), data.m_text.c_str( ) );
			}
		}
		ImGui::End( );

		ImGui::PopStyleVar( );
	}( GET_VARIABLE( g_variables.m_spectators_list, bool ) );
}

void n_misc::impl_t::practice_window_think( )
{
	if ( !GET_VARIABLE( g_variables.m_practice_window, bool ) )
		return;

	if ( !g_convars[ HASH_BT( "sv_cheats" ) ]->get_bool( ) )
		return;

	const auto cp_key = GET_VARIABLE( g_variables.m_practice_cp_key, key_bind_t ).m_key;

	const auto tp_key = GET_VARIABLE( g_variables.m_practice_tp_key, key_bind_t ).m_key;

	if ( g_input.is_key_released( cp_key ) ) {
		if ( !( g_ctx.m_local->get_flags( ) & fl_onground ) ) {
			g_logger.print( "you need to be onground to set a checkpoint.", "[practice]" );
			return;
		}

		c_angle saved_angles = { };
		g_interfaces.m_engine_client->get_view_angles( saved_angles );

		g_misc.practice.saved_angles   = saved_angles;
		g_misc.practice.saved_position = g_ctx.m_local->get_abs_origin( );

		g_logger.print( "saved checkpoint.", "[practice]" );
	} else if ( g_input.is_key_released( tp_key ) ) {
		if ( g_misc.practice.saved_angles.is_zero( ) || g_misc.practice.saved_position.is_zero( ) )
			return;

		g_interfaces.m_engine_client->client_cmd_unrestricted(
			std::string( "setpos " )
				.append( std::vformat( "{} {} {}", std::make_format_args( g_misc.practice.saved_position.m_x, g_misc.practice.saved_position.m_y,
		                                                                  g_misc.practice.saved_position.m_z ) ) )
				.append( ";setang " )
				.append( std::vformat( "{} {} {}", std::make_format_args( g_misc.practice.saved_angles.m_x, g_misc.practice.saved_angles.m_y,
		                                                                  g_misc.practice.saved_angles.m_z ) ) )
				.c_str( ) );
	}
}

void n_misc::impl_t::disable_post_processing( )
{
	g_convars[ HASH_BT( "mat_postprocess_enable" ) ]->set_value( !( GET_VARIABLE( g_variables.m_disable_post_processing, bool ) ) );
}

void n_misc::impl_t::remove_panorama_blur( )
{
	g_convars[ HASH_BT( "@panorama_disable_blur" ) ]->set_value( GET_VARIABLE( g_variables.m_remove_panorama_blur, bool ) );
}
