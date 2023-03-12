#include "menu.h"
#include "../../globals/fonts/fonts.h"
#include "../../globals/includes/includes.h"
#include "../../globals/logger/logger.h"
#include "../misc/scaleform/scaleform.h"

constexpr int color_picker_alpha_flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf |
                                         ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoInputs |
                                         ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar |
                                         ImGuiColorEditFlags_NoBorder;

constexpr int color_picker_no_alpha_flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_InputRGB |
                                            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop |
                                            ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder;

constexpr static const auto chams_materials = "flat\0textured\0metallic\0glow";

void save_popup( const char* str_id, const ImVec2& window_size, const std::function< void( ) >& fn )
{
	ImGuiContext& g         = *GImGui;
	const ImGuiStyle& style = g.Style;

	const auto window = g.CurrentWindow;

	const auto hashed_str_id = ImHashStr( str_id );
	const auto text_size     = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
        g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, str_id );

	const ImColor accent_color = ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent );

	ImGui::OpenPopup( str_id );

	ImGui::SetNextWindowSize( window_size );

	if ( ImGui::BeginPopup( str_id ) ) {
		const auto draw_list = ImGui::GetWindowDrawList( );

		const ImVec2 position = ImGui::GetWindowPos( ), size = ImGui::GetWindowSize( );

		auto text_animation = ImAnimationHelper( hashed_str_id, ImGui::GetIO( ).DeltaTime );

		ImGui::PushClipRect( ImVec2( position ), ImVec2( position.x + size.x, position.y + 20.f ), false );

		draw_list->AddRectFilled( ImVec2( position ), ImVec2( position.x + size.x, position.y + 20.f ), ImColor( 25 / 255.f, 25 / 255.f, 25 / 255.f ),
		                          g.Style.WindowRounding - 2.f, ImDrawFlags_RoundCornersTop );

		ImGui::PopClipRect( );

		ImGui::PushClipRect( position, ImVec2( position.x + size.x, position.y + size.y ), false );
		draw_list->AddRect( position, ImVec2( position.x + size.x, position.y + size.y ), ImColor( 50, 50, 50, 100 ), g.Style.WindowRounding - 2.f );

		text_animation.Update( ImGui::IsMouseHoveringRect( position, ImVec2( position.x + size.x, position.y + size.y ) ) ? 3.f : -2.f, 1.f, 0.5f,
		                       1.f );

		ImGui::PopClipRect( );

		const ImColor text_color = ImGui::GetColorU32( ImGuiCol_Text );

		draw_list->AddText(
			g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ], g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
			ImVec2( position.x + ( size.x - text_size.x ) / 2.f, position.y + ( 20.f - text_size.y ) / 2.f ),
			ImColor( text_color.Value.x, text_color.Value.y, text_color.Value.z, text_color.Value.w * text_animation.AnimationData->second ),
			str_id );

		RenderFadedGradientLine( draw_list, ImVec2( position.x, position.y + 20.f ), ImVec2( size.x, 1.f ),
		                         ImColor( accent_color.Value.x, accent_color.Value.y, accent_color.Value.z ) );

		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 20.f );

		fn( );

		ImGui::EndPopup( );
	}
}

void n_menu::impl_t::on_end_scene( )
{
	ImGui::GetStyle( ).Colors[ ImGuiCol_::ImGuiCol_Accent ] = GET_VARIABLE( g_variables.m_accent, c_color ).get_vec4( );

	if ( !this->m_opened )
		return;

	constexpr auto background_height = 25.f;

	ImGui::SetNextWindowSize( ImVec2( 588.f, 559.f ), ImGuiCond_::ImGuiCond_Always );

	ImGui::Begin( ( "hotwheels-ui" ), 0,
	              ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
	                  ImGuiWindowFlags_::ImGuiWindowFlags_NoResize );
	{
		const auto window = ImGui::GetCurrentWindow( );

		const auto draw_list = window->DrawList;

		const auto size     = window->Size;
		const auto position = window->Pos;

		static int tab_number = 0;

		[ & ]( ) {
			/* render background */
			ImGui::PushClipRect( ImVec2( position.x, position.y ), ImVec2( position.x + size.x, position.y + background_height ), false );
			draw_list->AddRectFilled( ImVec2( position.x, position.y ), ImVec2( position.x + size.x, position.y + background_height ),
			                          ImColor( 25 / 255.f, 25 / 255.f, 25 / 255.f, 1.f ), ImGui::GetStyle( ).WindowRounding,
			                          ImDrawFlags_RoundCornersTop );
			ImGui::PopClipRect( );

			ImGui::PushClipRect( ImVec2( position.x, position.y ), ImVec2( position.x + size.x, position.y + size.y ), false );
			draw_list->AddRect( ImVec2( position.x, position.y ), ImVec2( position.x + size.x, position.y + size.y ), ImColor( 50, 50, 50, 255 ),
			                    ImGui::GetStyle( ).WindowRounding );
			ImGui::PopClipRect( );

			/* render gradient */
			RenderFadedGradientLine( draw_list, ImVec2( position.x, position.y + background_height - 1.f ), ImVec2( size.x, 1.f ),
			                         ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent ) );

			const auto title_text      = ( "hotwheels" );
			const auto title_text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
				g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, title_text );

			const auto vip_title_text      = ( ".vip" );
			const auto vip_title_text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
				g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, vip_title_text );

			draw_list->AddText( g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
			                    g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
			                    ImVec2( position.x + ( ( size.x - title_text_size.x - vip_title_text_size.x ) / 2.f ),
			                            position.y + ( ( background_height - title_text_size.y ) / 2.f ) ),
			                    ImColor( 1.f, 1.f, 1.f ), title_text );

			draw_list->AddText( g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
			                    g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
			                    ImVec2( position.x + ( ( size.x + title_text_size.x - vip_title_text_size.x ) / 2.f ),
			                            position.y + ( ( background_height - title_text_size.y ) / 2.f ) ),
			                    ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent ), vip_title_text );
		}( );

		[ & ]( ) {
			/* render background */
			ImGui::PushClipRect( ImVec2( position.x, position.y + size.y - background_height ), ImVec2( position.x + size.x, position.y + size.y ),
			                     false );
			draw_list->AddRectFilled( ImVec2( position.x, position.y + size.y - background_height ),
			                          ImVec2( position.x + size.x, position.y + size.y ), ImColor( 25 / 255.f, 25 / 255.f, 25 / 255.f ),
			                          ImGui::GetStyle( ).WindowRounding, ImDrawFlags_RoundCornersBottom );
			ImGui::PopClipRect( );

			/* render gradient */
			RenderFadedGradientLine( draw_list, ImVec2( position.x, position.y + size.y - background_height ), ImVec2( size.x, 1.f ),
			                         ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent ) );

			std::vector< const char* > tab_names = { ( "aimbot" ), ( "visuals" ), ( "movement" ), ( "misc" ), ( "inventory" ), ( "settings" ) };

			/* tab logic */
			for ( int iterator = { }; iterator < static_cast< int >( tab_names.size( ) ); iterator++ ) {
				if ( !( iterator < static_cast< int >( tab_names.size( ) ) ) )
					break;

				const char* const tab_name = tab_names[ iterator ];

				const auto hashed_tab_name = ImHashStr( tab_name );

				const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, tab_name );

				const int tab_width    = ( size.x / static_cast< int >( tab_names.size( ) ) );
				const int tab_center_x = ( tab_width * iterator ) + ( tab_width / 2 );

				const auto text_position = ImVec2( tab_center_x - ( text_size.x / 2 ), ( size.y - ( background_height + text_size.y ) / 2.f ) - 1.f );

				const bool hovered =
					ImGui::IsMouseHoveringRect( ImVec2( position.x + text_position.x, position.y + text_position.y ),
				                                ImVec2( position.x + text_position.x + text_size.x, position.y + text_position.y + text_size.y ) );

				const bool selected = iterator == tab_number;

				auto hovered_text_animation = ImAnimationHelper( hashed_tab_name + ImHashStr( "hovered-text-animation" ), ImGui::GetIO( ).DeltaTime );
				hovered_text_animation.Update( 2.f, hovered ? 2.f : -2.f, 0.5f );

				auto selected_animation = ImAnimationHelper( hashed_tab_name + ImHashStr( "selected-animation" ), ImGui::GetIO( ).DeltaTime );
				selected_animation.Update( 2.f, selected ? 2.f : -2.f );

				draw_list->AddText( g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
				                    g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
				                    ImVec2( position.x + text_position.x, position.y + text_position.y ),
				                    ImColor::Blend( ImColor( 1.f, 1.f, 1.f, hovered_text_animation.AnimationData->second ),
				                                    ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent ), selected_animation.AnimationData->second ),
				                    tab_name );

				if ( hovered && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) )
					tab_number = iterator;
			}
		}( );

		/* set next working position for the elements */
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 25.f );

		switch ( tab_number ) {
		case 0: /* aimbot */ {
			if ( ImGui::BeginChild(
					 ( "main" ), ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
					 true, 0, true ) ) {
				ImGui::Checkbox( "enable aimbot", &GET_VARIABLE( g_variables.m_aimbot_enable, bool ) );
				ImGui::Checkbox( "enable backtrack", &GET_VARIABLE( g_variables.m_backtrack_enable, bool ) );
				if ( GET_VARIABLE( g_variables.m_backtrack_enable, bool ) ) {
					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "extended", &GET_VARIABLE( g_variables.m_backtrack_extend, bool ) );
				}
			}
			ImGui::EndChild( );
			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild( ( "configuration" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) { }
			ImGui::EndChild( );
			break;
		}
		case 1: /* visuals */ {
			static int esp_subtab_number = 0;

			if ( ImGui::BeginChild(
					 ( "esp" ), { "players", "edicts" }, &esp_subtab_number,
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ), true,
					 0, true ) ) {
				switch ( esp_subtab_number ) {
				case 0: {
					ImGui::Checkbox( "players", &GET_VARIABLE( g_variables.m_players, bool ) );
					if ( GET_VARIABLE( g_variables.m_players, bool ) ) {
						ImGui::Checkbox( "bounding box##player", &GET_VARIABLE( g_variables.m_players_box, bool ) );
						if ( GET_VARIABLE( g_variables.m_players_box, bool ) ) {
							ImGui::ColorEdit4( "##player bounding box color", &GET_VARIABLE( g_variables.m_players_box_color, c_color ),
							                   color_picker_alpha_flags );

							if ( GET_VARIABLE( g_variables.m_players_box_outline, bool ) ) {
								ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

								ImGui::ColorEdit4( "##player bounding box outline color",
								                   &GET_VARIABLE( g_variables.m_players_box_outline_color, c_color ), color_picker_alpha_flags );
							}

							ImGui::SetCursorPosX( 26.f );
							ImGui::Checkbox( "corner bounding box##player", &GET_VARIABLE( g_variables.m_players_box_corner, bool ) );
							ImGui::SetCursorPosX( 26.f );
							ImGui::Checkbox( "bounding box outline##player", &GET_VARIABLE( g_variables.m_players_box_outline, bool ) );
						}

						ImGui::Checkbox( "name##player", &GET_VARIABLE( g_variables.m_players_name, bool ) );
						if ( GET_VARIABLE( g_variables.m_players_name, bool ) )
							ImGui::ColorEdit4( "##player name color", &GET_VARIABLE( g_variables.m_players_name_color, c_color ),
							                   color_picker_alpha_flags );

						ImGui::Checkbox( "health bar##player", &GET_VARIABLE( g_variables.m_players_health_bar, bool ) );
						if ( GET_VARIABLE( g_variables.m_players_health_bar, bool ) ) {
							ImGui::SetCursorPosX( 26.f );
							ImGui::Checkbox( "custom health bar color", &GET_VARIABLE( g_variables.m_players_health_bar_custom_color, bool ) );
							ImGui::ColorEdit4( "##health bar color", &GET_VARIABLE( g_variables.m_players_health_bar_color, c_color ),
							                   color_picker_alpha_flags );
						}

						ImGui::Checkbox( "health number##player", &GET_VARIABLE( g_variables.m_players_health_text, bool ) );
						// TODO add more types
						if ( GET_VARIABLE( g_variables.m_players_health_text, bool ) ) {
							ImGui::OptionPopup(
								"health number settings##player health number settings",
								[ & ]( ) {
									ImGui::Checkbox( "health number suffix", &GET_VARIABLE( g_variables.m_players_health_suffix, bool ) );
									ImGui::Combo( "health number style", &GET_VARIABLE( g_variables.m_players_health_text_style, int ),
								                  "standalone bottom\0follow health bar" );
								},
								ImVec2( 200.f, -1 ) );
						}

						ImGui::Checkbox( "weapon name##player", &GET_VARIABLE( g_variables.m_weapon_name, bool ) );
						if ( GET_VARIABLE( g_variables.m_weapon_name, bool ) )
							ImGui::ColorEdit4( "##weapon name color", &GET_VARIABLE( g_variables.m_weapon_name_color, c_color ),
							                   color_picker_alpha_flags );

						ImGui::Checkbox( "weapon icon##player", &GET_VARIABLE( g_variables.m_weapon_icon, bool ) );
						if ( GET_VARIABLE( g_variables.m_weapon_icon, bool ) )
							ImGui::ColorEdit4( "##weapon icon color", &GET_VARIABLE( g_variables.m_weapon_icon_color, c_color ),
							                   color_picker_alpha_flags );

						ImGui::Checkbox( "weapon ammo bar##player", &GET_VARIABLE( g_variables.m_player_ammo_bar, bool ) );
						if ( GET_VARIABLE( g_variables.m_player_ammo_bar, bool ) )
							ImGui::ColorEdit4( "##weapon icon color", &GET_VARIABLE( g_variables.m_player_ammo_bar_color, c_color ),
							                   color_picker_alpha_flags );

						ImGui::Checkbox( "player skeleton", &GET_VARIABLE( g_variables.m_players_skeleton, bool ) );
						if ( GET_VARIABLE( g_variables.m_players_skeleton, bool ) ) {
							ImGui::ColorEdit4( "##player skeleton color", &GET_VARIABLE( g_variables.m_players_skeleton_color, c_color ),
							                   color_picker_alpha_flags );
							ImGui::Combo( "skeleton type##player skeleton", &GET_VARIABLE( g_variables.m_players_skeleton_type, int ),
							              "normal\0lag compensated" );
						}
						ImGui::Checkbox( "lag compensated trail", &GET_VARIABLE( g_variables.m_players_backtrack_trail, bool ) );
						ImGui::Checkbox( "players avatar", &GET_VARIABLE( g_variables.m_players_avatar, bool ) );

						ImGui::Checkbox( ( "out of fov arrows" ), &GET_VARIABLE( g_variables.m_out_of_fov_arrows, bool ) );
						if ( GET_VARIABLE( g_variables.m_out_of_fov_arrows, bool ) ) {
							ImGui::OptionPopup(
								"fov arrows settings",
								[ & ]( ) {
									ImGui::SetCursorPosX( 25.f );
									ImGui::Text( "fov arrows color" );
									ImGui::ColorEdit4( ( "##out of fov arrows color" ),
								                       &GET_VARIABLE( g_variables.m_out_of_fov_arrows_color, c_color ), color_picker_alpha_flags );
									ImGui::SliderFloat( "arrows size", &GET_VARIABLE( g_variables.m_out_of_fov_arrows_size, float ), 0.1f, 50.f,
								                        "%.1f px" );
									ImGui::SliderInt( "arrows distance", &GET_VARIABLE( g_variables.m_out_of_fov_arrows_distance, int ), 10, 500,
								                      "%d px" );
								},
								ImVec2( 200.f, -1 ) );
						}
					}
					break;
				}
				case 1: {
					ImGui::Checkbox( "dropped weapons", &GET_VARIABLE( g_variables.m_dropped_weapons, bool ) );
					if ( GET_VARIABLE( g_variables.m_dropped_weapons, bool ) ) {
						ImGui::OptionPopup(
							"dropped weapons settings",
							[ & ]( ) {
								ImGui::Checkbox( "bounding box##dropped weapons", &GET_VARIABLE( g_variables.m_dropped_weapons_box, bool ) );
								if ( GET_VARIABLE( g_variables.m_dropped_weapons_box, bool ) ) {
									if ( GET_VARIABLE( g_variables.m_dropped_weapons_box_outline, bool ) ) {
										ImGui::ColorEdit4( "##dropped weapons bounding box color",
									                       &GET_VARIABLE( g_variables.m_dropped_weapons_box_color, c_color ),
									                       color_picker_alpha_flags );

										ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

										ImGui::ColorEdit4( "##dropped weapons bounding box outline color",
									                       &GET_VARIABLE( g_variables.m_dropped_weapons_box_outline_color, c_color ),
									                       color_picker_alpha_flags );
									}

									ImGui::SetCursorPosX( 26.f );
									ImGui::Checkbox( "corner bounding box##dropped weapons",
								                     &GET_VARIABLE( g_variables.m_dropped_weapons_box_corner, bool ) );
									ImGui::SetCursorPosX( 26.f );
									ImGui::Checkbox( "bounding box outline##dropped weapons",
								                     &GET_VARIABLE( g_variables.m_dropped_weapons_box_outline, bool ) );
								}

								ImGui::Checkbox( "name##dropped weapons", &GET_VARIABLE( g_variables.m_dropped_weapons_name, bool ) );
								if ( GET_VARIABLE( g_variables.m_dropped_weapons_name, bool ) )
									ImGui::ColorEdit4( "##dropped weapons name color",
								                       &GET_VARIABLE( g_variables.m_dropped_weapons_name_color, c_color ), color_picker_alpha_flags );

								ImGui::Checkbox( "icons##dropped weapons icon", &GET_VARIABLE( g_variables.m_dropped_weapons_icon, bool ) );
								if ( GET_VARIABLE( g_variables.m_dropped_weapons_icon, bool ) )
									ImGui::ColorEdit4( "##dropped weapons name color",
								                       &GET_VARIABLE( g_variables.m_dropped_weapons_icon_color, c_color ), color_picker_alpha_flags );
							},
							ImVec2( 200.f, -1 ) );
					}

					ImGui::Checkbox( "thrown projectiles", &GET_VARIABLE( g_variables.m_thrown_objects, bool ) );
					if ( GET_VARIABLE( g_variables.m_thrown_objects, bool ) ) {
						ImGui::OptionPopup(
							"health number settings",
							[ & ]( ) {
								ImGui::Checkbox( "thrown objects name", &GET_VARIABLE( g_variables.m_thrown_objects_name, bool ) );
								if ( GET_VARIABLE( g_variables.m_thrown_objects_name, bool ) )
									ImGui::ColorEdit4( "##thrown objects name color",
								                       &GET_VARIABLE( g_variables.m_thrown_objects_name_color, c_color ), color_picker_alpha_flags );
								ImGui::Checkbox( "thrown objects icon", &GET_VARIABLE( g_variables.m_thrown_objects_icon, bool ) );
								if ( GET_VARIABLE( g_variables.m_thrown_objects_icon, bool ) )
									ImGui::ColorEdit4( "##thrown objects icon color",
								                       &GET_VARIABLE( g_variables.m_thrown_objects_icon_color, c_color ), color_picker_alpha_flags );
							},
							ImVec2( 200.f, -1 ) );
					}
					break;
				}
				}

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild(
					 ( "chams" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ),
					 true, 0, true ) ) {
				ImGui::Checkbox( "visible chams", &GET_VARIABLE( g_variables.m_player_visible_chams, bool ) );
				if ( GET_VARIABLE( g_variables.m_player_visible_chams, bool ) ) {
					ImGui::ColorEdit4( "visible chams color##visible chams color", &GET_VARIABLE( g_variables.m_player_visible_chams_color, c_color ),
					                   color_picker_alpha_flags );

					ImGui::Combo( "visible chams material", &GET_VARIABLE( g_variables.m_player_visible_chams_material, int ), chams_materials );
				}

				ImGui::Checkbox( "invisible chams", &GET_VARIABLE( g_variables.m_player_invisible_chams, bool ) );
				if ( GET_VARIABLE( g_variables.m_player_invisible_chams, bool ) ) {
					ImGui::ColorEdit4( "invisible chams color##invisible chams color",
					                   &GET_VARIABLE( g_variables.m_player_invisible_chams_color, c_color ), color_picker_alpha_flags );

					ImGui::Combo( "invisible chams material", &GET_VARIABLE( g_variables.m_player_invisible_chams_material, int ), chams_materials );
				}

				ImGui::Checkbox( "lag compensated chams", &GET_VARIABLE( g_variables.m_player_lag_chams, bool ) );
				if ( GET_VARIABLE( g_variables.m_player_lag_chams, bool ) ) {
					ImGui::ColorEdit4( "lag compensated chams color##lagcomp chams color",
					                   &GET_VARIABLE( g_variables.m_player_lag_chams_color, c_color ), color_picker_alpha_flags );
					ImGui::Checkbox( "lag compensated chams xqz", &GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );
					ImGui::Combo( "lag compensated chams material", &GET_VARIABLE( g_variables.m_player_lag_chams_material, int ), chams_materials );
					ImGui::Combo( "lag compensated chams type", &GET_VARIABLE( g_variables.m_player_lag_chams_type, int ),
					              "oldest record\0all records\0aimbot target record" );
				}
			}
			ImGui::EndChild( );

			if ( ImGui::BeginChild(
					 ( "glow" ), ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
					 true, 0, true ) ) {
				ImGui::Checkbox( "player glow", &GET_VARIABLE( g_variables.m_glow_enable, bool ) );
				ImGui::ColorEdit4( "player visible##player vis glow color", &GET_VARIABLE( g_variables.m_glow_vis_color, c_color ),
				                   color_picker_alpha_flags );

				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

				ImGui::ColorEdit4( "player invisible##player invis glow color", &GET_VARIABLE( g_variables.m_glow_invis_color, c_color ),
				                   color_picker_alpha_flags );

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			static int other_subtab_number = 0;
			if ( ImGui::BeginChild( ( "other" ), { "effects", "removals" }, &other_subtab_number,
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				switch ( other_subtab_number ) {
				case 0: {
					ImGui::Checkbox( "precipitation", &GET_VARIABLE( g_variables.m_precipitation, bool ) );
					if ( GET_VARIABLE( g_variables.m_precipitation, bool ) ) {
						ImGui::ColorEdit4( "##precipitation color", &GET_VARIABLE( g_variables.m_precipitation_color, c_color ),
						                   color_picker_alpha_flags );

						ImGui::Combo( "type##precipitation", &GET_VARIABLE( g_variables.m_precipitation_type, int ), "rain\0ash\0rain storm\0snow" );
					}

					ImGui::Checkbox( "fog", &GET_VARIABLE( g_variables.m_fog, bool ) );
					if ( GET_VARIABLE( g_variables.m_fog, bool ) ) {
						ImGui::ColorEdit4( "##fog color picker", &GET_VARIABLE( g_variables.m_fog_color, c_color ), color_picker_alpha_flags );

						ImGui::SliderFloat( "start##fog", &GET_VARIABLE( g_variables.m_fog_start, float ), 0.f, 5000.f, "%.0f u" );
						ImGui::SliderFloat( "end##fog", &GET_VARIABLE( g_variables.m_fog_end, float ), 0.f, 5000.f, "%.0f u" );
					}

					ImGui::Checkbox( "custom smoke color", &GET_VARIABLE( g_variables.m_custom_smoke, bool ) );
					if ( GET_VARIABLE( g_variables.m_custom_smoke, bool ) )
						ImGui::ColorEdit4( "##custom smoke color picker", &GET_VARIABLE( g_variables.m_custom_smoke_color, c_color ),
						                   color_picker_alpha_flags );

					ImGui::Checkbox( "custom molotov color", &GET_VARIABLE( g_variables.m_custom_molotov, bool ) );
					if ( GET_VARIABLE( g_variables.m_custom_molotov, bool ) )
						ImGui::ColorEdit4( "##custom molotov color picker", &GET_VARIABLE( g_variables.m_custom_molotov_color, c_color ),
						                   color_picker_alpha_flags );

					ImGui::Checkbox( "custom blood color", &GET_VARIABLE( g_variables.m_custom_blood, bool ) );
					if ( GET_VARIABLE( g_variables.m_custom_blood, bool ) )
						ImGui::ColorEdit4( "##custom blood color picker", &GET_VARIABLE( g_variables.m_custom_blood_color, c_color ),
						                   color_picker_alpha_flags );

					break;
				}
				case 1: {
					ImGui::Checkbox( "disable post processing", &GET_VARIABLE( g_variables.m_disable_post_processing, bool ) );
					ImGui::Checkbox( "remove panorama blur", &GET_VARIABLE( g_variables.m_remove_panorama_blur, bool ) );
#ifdef _DEBUG
					ImGui::Checkbox( "disable interp", &GET_VARIABLE( g_variables.m_disable_interp, bool ) );
#endif
					break;
				}
				}

				ImGui::EndChild( );
			}
			break;
		}
		case 2: /* movement */ {
			if ( ImGui::BeginChild(
					 ( "movement" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ), true, 0,
					 true ) ) {
				ImGui::Checkbox( "bunny hop", &GET_VARIABLE( g_variables.m_bunny_hop, bool ) );

				ImGui::Checkbox( "edge jump", &GET_VARIABLE( g_variables.m_edge_jump, bool ) );
				ImGui::Keybind( "edge jump key", &GET_VARIABLE( g_variables.m_edge_jump_key, key_bind_t ) );

				if ( GET_VARIABLE( g_variables.m_edge_jump, bool ) ) {
					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "on ladders", &GET_VARIABLE( g_variables.m_edge_jump_ladder, bool ) );
				}

				ImGui::Checkbox( "long jump", &GET_VARIABLE( g_variables.m_long_jump, bool ) );
				ImGui::Keybind( "long jump key", &GET_VARIABLE( g_variables.m_long_jump_key, key_bind_t ) );

				ImGui::Checkbox( "mini jump", &GET_VARIABLE( g_variables.m_mini_jump, bool ) );
				ImGui::Keybind( "mini jump key", &GET_VARIABLE( g_variables.m_mini_jump_key, key_bind_t ) );
				if ( GET_VARIABLE( g_variables.m_mini_jump, bool ) ) {
					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "hold crouch", &GET_VARIABLE( g_variables.m_mini_jump_hold_duck, bool ) );
				}

				ImGui::Checkbox( "jump bug", &GET_VARIABLE( g_variables.m_jump_bug, bool ) );
				ImGui::Keybind( "jump bug key", &GET_VARIABLE( g_variables.m_jump_bug_key, key_bind_t ) );

				ImGui::Checkbox( "edge bug", &GET_VARIABLE( g_variables.m_edge_bug, bool ) );

				if ( GET_VARIABLE( g_variables.m_edge_bug, bool ) ) {
					ImGui::OptionPopup(
						"edgebug advanced settings",
						[ & ]( ) {
							ImGui::SetCursorPosX( 25.f );
							ImGui::Text( "edgebug keybind" );
							ImGui::Keybind( "edge bug key", &GET_VARIABLE( g_variables.m_edge_bug_key, key_bind_t ) );
							ImGui::SliderInt( "edge bug predicted ticks", &GET_VARIABLE( g_variables.m_edge_bug_ticks, int ), 0, 32 );
							ImGui::SliderFloat( "mouse lock scale", &GET_VARIABLE( g_variables.m_edge_bug_lock_amt, float ), 0.f, 1.f, "%.2f" );

							ImGui::Checkbox( "advanced detection", &GET_VARIABLE( g_variables.m_advanced_detection, bool ) );
							if ( GET_VARIABLE( g_variables.m_advanced_detection, bool ) )
								ImGui::SliderFloat( "max strafe delta", &GET_VARIABLE( g_variables.m_edge_bug_strafe_delta_max, float ), 0.1f, 100.f,
							                        "%.1f%" );
						},
						ImVec2( 200.f, -1 ) );
				}
				ImGui::Checkbox( "pixel surf", &GET_VARIABLE( g_variables.m_pixel_surf, bool ) );
				ImGui::Keybind( "pixel surf key", &GET_VARIABLE( g_variables.m_pixel_surf_key, key_bind_t ) );

				ImGui::Checkbox( "auto align", &GET_VARIABLE( g_variables.m_auto_align, bool ) );

				ImGui::Checkbox( "no crouch cooldown", &GET_VARIABLE( g_variables.m_no_crouch_cooldown, bool ) );

				ImGui::Checkbox( "auto duck", &GET_VARIABLE( g_variables.m_auto_duck, bool ) );

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild( ( "indicators" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::Checkbox( "velocity indicator", &GET_VARIABLE( g_variables.m_velocity_indicator, bool ) );
				if ( GET_VARIABLE( g_variables.m_velocity_indicator, bool ) ) {
					ImGui::OptionPopup(
						"velocity indicator settings",
						[ & ]( ) {
							ImGui::Text( "velocity colors" );

							if ( GET_VARIABLE( g_variables.m_velocity_indicator_custom_color, bool ) ) {
								ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) );

								ImGui::ColorEdit4( "##velocity indicator color 1", &GET_VARIABLE( g_variables.m_velocity_indicator_color1, c_color ),
							                       color_picker_alpha_flags );

								ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

								ImGui::ColorEdit4( "##velocity indicator color 2", &GET_VARIABLE( g_variables.m_velocity_indicator_color2, c_color ),
							                       color_picker_alpha_flags );
							} else {
								ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) );

								ImGui::ColorEdit4( "##velocity indicator color 3", &GET_VARIABLE( g_variables.m_velocity_indicator_color3, c_color ),
							                       color_picker_alpha_flags );

								ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

								ImGui::ColorEdit4( "##velocity indicator color 4", &GET_VARIABLE( g_variables.m_velocity_indicator_color4, c_color ),
							                       color_picker_alpha_flags );

								ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 50.f );

								ImGui::ColorEdit4( "##velocity indicator color 5", &GET_VARIABLE( g_variables.m_velocity_indicator_color5, c_color ),
							                       color_picker_alpha_flags );
							}

							ImGui::Checkbox( "show pre speed##velocity indicator",
						                     &GET_VARIABLE( g_variables.m_velocity_indicator_show_pre_speed, bool ) );

							ImGui::Checkbox( "fade alpha##velocity indicator", &GET_VARIABLE( g_variables.m_velocity_indicator_fade_alpha, bool ) );

							ImGui::Checkbox( "custom color##velocity indicator",
						                     &GET_VARIABLE( g_variables.m_velocity_indicator_custom_color, bool ) );

							ImGui::SliderInt( "padding##velocity indicator", &GET_VARIABLE( g_variables.m_velocity_indicator_padding, int ), 5, 100,
						                      "%d%%", 0, false );
						},
						ImVec2( 200.f, -1 ) );
				}

				ImGui::Checkbox( "stamina indicator", &GET_VARIABLE( g_variables.m_stamina_indicator, bool ) );
				if ( GET_VARIABLE( g_variables.m_stamina_indicator, bool ) ) {
					ImGui::OptionPopup(
						"stamina indicator settings",
						[ & ]( ) {
							ImGui::Text( "stamina colors" );

							ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) );

							ImGui::ColorEdit4( "##stamina indicator color 1", &GET_VARIABLE( g_variables.m_stamina_indicator_color1, c_color ),
						                       color_picker_alpha_flags );

							ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

							ImGui::ColorEdit4( "##stamina indicator color 2", &GET_VARIABLE( g_variables.m_stamina_indicator_color2, c_color ),
						                       color_picker_alpha_flags );

							ImGui::Checkbox( "show pre speed##stamina indicator",
						                     &GET_VARIABLE( g_variables.m_stamina_indicator_show_pre_speed, bool ) );

							ImGui::Checkbox( "fade alpha##stamina indicator", &GET_VARIABLE( g_variables.m_stamina_indicator_fade_alpha, bool ) );

							ImGui::SliderInt( "padding##stamina indicator", &GET_VARIABLE( g_variables.m_stamina_indicator_padding, int ), 5, 100,
						                      "%d%%", 0, false );
						},
						ImVec2( 200.f, -1 ) );
				}

				ImGui::Checkbox( "keybind indicators", &GET_VARIABLE( g_variables.m_key_indicators_enable, bool ) );
				if ( GET_VARIABLE( g_variables.m_key_indicators_enable, bool ) ) {
					ImGui::ColorEdit4( "##keybind color", &GET_VARIABLE( g_variables.m_key_color, c_color ), color_picker_alpha_flags );

					ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

					ImGui::ColorEdit4( "##keybind success color", &GET_VARIABLE( g_variables.m_key_color_success, c_color ),
					                   color_picker_alpha_flags );

					ImGui::SliderInt( "position##sub indicators", &GET_VARIABLE( g_variables.m_key_indicators_position, int ), 30, g_ctx.m_height );

					ImGui::MultiCombo( "displayed keybinds", GET_VARIABLE( g_variables.m_key_indicators, std::vector< bool > ),
					                   { "edgebug", "pixelsurf", "edgejump", "longjump", "delayhop", "minijump", "jumpbug" },
					                   GET_VARIABLE( g_variables.m_key_indicators, std::vector< bool > ).size( ) );
				}

				ImGui::EndChild( );
			}
			break;
		}
		case 3: /* misc */
		{
			if ( ImGui::BeginChild(
					 ( "game" ), ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
					 true, 0, true ) ) {
				ImGui::Checkbox( "spectator list", &GET_VARIABLE( g_variables.m_spectators_list, bool ) );

				if ( GET_VARIABLE( g_variables.m_spectators_list, bool ) ) {
					ImGui::OptionPopup(
						"spectators configuration",
						[]( ) {
							ImGui::SetCursorPosX( 25.f );
							ImGui::Text( "spectating local color" );
							ImGui::ColorEdit4( "##spectator list text color one",
						                       &GET_VARIABLE( g_variables.m_spectators_list_text_color_one, c_color ), color_picker_alpha_flags );
							ImGui::SetCursorPosX( 25.f );
							ImGui::Text( "spectating other color" );
							ImGui::ColorEdit4( "##spectator list text color two",
						                       &GET_VARIABLE( g_variables.m_spectators_list_text_color_two, c_color ), color_picker_alpha_flags );

							ImGui::Combo( "listed players##spectatorlist player", &GET_VARIABLE( g_variables.m_spectators_list_type, int ),
						                  "all spectators\0local spectators\0" );

							ImGui::Checkbox( "spectator avatars", &GET_VARIABLE( g_variables.m_spectators_avatar, bool ) );
						},
						ImVec2( 200.f, -1.f ) );
				}
				// TODO: combobox between force crosshair and just draw a white dot
				ImGui::Checkbox( "sniper crosshair", &GET_VARIABLE( g_variables.m_sniper_crosshair, bool ) );

				ImGui::Checkbox( "practice window", &GET_VARIABLE( g_variables.m_practice_window, bool ) );

				if ( GET_VARIABLE( g_variables.m_practice_window, bool ) ) {
					ImGui::Text( "practice checkpoint key" );
					ImGui::Keybind( "practice cp key", &GET_VARIABLE( g_variables.m_practice_cp_key, key_bind_t ) );
					ImGui::Text( "practice teleport key" );
					ImGui::Keybind( "practice tp key", &GET_VARIABLE( g_variables.m_practice_tp_key, key_bind_t ) );
				}

				ImGui::MultiCombo( "displayed logs", GET_VARIABLE( g_variables.m_log_types, std::vector< bool > ),
				                   { "damage", "team damage", "purchase", "votes" },
				                   GET_VARIABLE( g_variables.m_log_types, std::vector< bool > ).size( ) );

				ImGui::Checkbox( "scaleform", &GET_VARIABLE( g_variables.m_scaleform, bool ) );
				if ( ImGui::Button( "force hud update" ) )
					g_scaleform.m_should_force_update = true;

#ifdef _DEBUG

				static int ctx_panel = 0;
				static char buffer[ 1024 * 16 ];
				// ImGui::InputTextMultiline( "panorama debug text##panorama debug", buffer, IM_ARRAYSIZE( buffer ) );
				//  ^ this does not work for some reason. code for it is fucked and i dont feel like fixing
				ImGui::InputText( "panorama script text", buffer, IM_ARRAYSIZE( buffer ) );

				ImGui::Combo( "context panel#panel", &ctx_panel, "CSGOHud\0CSGOMainMenu" );

				if ( ImGui::Button( "run panorama script" ) )
					g_scaleform.m_uiengine->run_script( ctx_panel == 0 ? g_scaleform.m_hud_panel : g_scaleform.m_menu_panel, buffer,
					                                    ctx_panel == 0 ? "panorama/layout/hud/hud.xml" : "panorama/layout/mainmenu.xml", 8, 10, false,
					                                    false );

#endif

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild( ( "exploits" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::EndChild( );
			}
			break;
		}
		case 4: /* inventory */ {
			break;
		}
		case 5: /* settings */
		{
			if ( ImGui::BeginChild(
					 ( "settings" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ), true,
					 0, true ) ) {
				ImGui::InputText( ( "config file name" ), this->m_config_file, sizeof( this->m_config_file ),0,0,0 ,-7.f);

				std::string converted_file_name = this->m_config_file;

				static std::string selected_config_name = { };

				ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail( ).x - 2.f );

				ImGui::PushStyleVar( ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2( 0.f, 3.f ) );

				ImGui::ListBox( ( "##config list" ), &this->m_selected_config, []( int index ) { return g_config.m_file_names[ index ].c_str( ); },
				                g_config.m_file_names.size( ), 5 );

				ImGui::PopStyleVar( );

				selected_config_name = !g_config.m_file_names.empty( ) ? g_config.m_file_names[ this->m_selected_config ] : "";

				if ( ImGui::Button( ( "create" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					if ( !g_config.save( converted_file_name ) )
						g_console.print( std::vformat( "failed to create {:s}", std::make_format_args( converted_file_name ) ).c_str( ) );
					else
						g_logger.print( std::vformat( "created config {:s}", std::make_format_args( converted_file_name ) ).c_str( ) );

					converted_file_name.clear( );
					g_config.refresh( );
				}

				static bool open_save_popup = false;
				if ( ImGui::Button( ( "save" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					open_save_popup = true;

				if ( open_save_popup ) {
					save_popup( "save confirmation", ImVec2( 220.f, -1.f ), []( ) {
						if ( ImGui::Button( "yes", ImVec2( ( ImGui::GetContentRegionAvail( ).x - 33.f ) / 2.f, 15.f ) ) ) {
							if ( !g_config.save( selected_config_name ) )
								g_console.print( std::vformat( "failed to save {:s}", std::make_format_args( selected_config_name ) ).c_str( ) );
							else
								g_logger.print( std::vformat( "saved config {:s}", std::make_format_args( selected_config_name ) ).c_str( ) );

							open_save_popup = false;
						}

						ImGui::SameLine( );

						if ( ImGui::Button( "no", ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
							g_logger.print( std::vformat( "canceled saving config {:s}", std::make_format_args( selected_config_name ) ).c_str( ) );

							open_save_popup = false;
						}
					} );
				}

				if ( ImGui::Button( ( "load" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					if ( !g_config.load( selected_config_name ) )
						g_console.print( std::vformat( "failed to load {:s}", std::make_format_args( selected_config_name ) ).c_str( ) );
					else {
						g_logger.print( std::vformat( "loaded config {:s}", std::make_format_args( selected_config_name ) ).c_str( ) );
						g_render.m_reload_fonts = true;
					}
				}

				if ( ImGui::Button( ( "remove" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					g_logger.print( std::vformat( "removed config {:s}", std::make_format_args( selected_config_name ) ).c_str( ) );

					g_config.remove( this->m_selected_config );
					this->m_selected_config = 0;
				}

				if ( ImGui::Button( ( "refresh" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					g_config.refresh( );

				ImGui::SetCursorPosX( ImGui::GetCursorPos( ).x + 17.f );
				ImGui::Text( "accent color" );

				ImGui::ColorEdit4( ( "##accent color" ), &GET_VARIABLE( g_variables.m_accent, c_color ), color_picker_no_alpha_flags );
#ifdef _DEBUG
				ImGui::Checkbox( "watermark", &GET_VARIABLE( g_variables.m_watermark, bool ) );
				ImGui::Checkbox( "debugger menu", &GET_VARIABLE( g_variables.m_debugger_visual, bool ) );
#endif

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild(
					 ( "lua" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ),
					 true, 0, true ) ) {
				ImGui::EndChild( );
			}

			static int font_subtab_number = 0;

			if ( ImGui::BeginChild( ( "fonts" ), { "indicator", "name", "flags", "weapon", "dropped weapon" }, &font_subtab_number,
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				switch ( font_subtab_number ) {
				case 0: {
					ImGui::SliderInt( "size##indicator font", &GET_VARIABLE( g_variables.m_indicator_font_settings, font_setting_t ).m_size, 0, 50,
					                  "%d", 0, false );

					if ( ImGui::Button( "reset to default##indicator font", ImVec2( ( ImGui::GetContentRegionAvail( ).x - 5.f ), 15.f ), false ) ) {
						g_render.m_custom_fonts[ e_custom_font_names::custom_font_name_indicator ] =
							g_render.m_fonts[ e_font_names::font_name_indicator_29 ];
					}

					ImGui::SetCursorPosX( -10.f );

					static ImGuiTextFilter indicator_font_search_filter{ };
					indicator_font_search_filter.Draw( "search for a font", 20.f);

					ImGui::SetCursorPosX( -10.f );
					if ( ImGui::BeginListBox( "##font list", ImVec2( ImGui::GetContentRegionAvail( ).x + 8.f, 100.f ) ) ) {
						for ( const auto iterator : g_fonts.m_font_file_names ) {
							if ( indicator_font_search_filter.PassFilter( iterator.c_str( ) ) )
								if ( ImGui::Selectable(
										 iterator.c_str( ),
										 HASH_RT( GET_VARIABLE( g_variables.m_indicator_font_settings, font_setting_t ).m_name.c_str( ) ) ==
											 HASH_RT( iterator.c_str( ) ) ) )
									GET_VARIABLE( g_variables.m_indicator_font_settings, font_setting_t ).m_name = iterator;
						}
						ImGui::EndListBox( );
					}

					break;
				}
				}

				if ( ImGui::Button( "reload fonts", ImVec2( ( ImGui::GetContentRegionAvail( ).x - 5.f ), 15.f ), false ) )
					g_render.m_reload_fonts = true;

				ImGui::EndChild( );
			}
			break;
		}
		}
	}
	ImGui::End( );
}
