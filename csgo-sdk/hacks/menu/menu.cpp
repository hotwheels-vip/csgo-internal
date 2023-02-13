#include "menu.h"
#include "../../globals/includes/includes.h"

constexpr int color_picker_alpha_flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf |
                                         ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoInputs |
                                         ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar |
                                         ImGuiColorEditFlags_NoBorder;

constexpr int color_picker_no_alpha_flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_InputRGB |
                                            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop |
                                            ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder;

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

			std::vector< const char* > tab_names = { ( "aimbot" ), ( "visuals" ), ( "movement" ), ( "misc" ), ( "skins" ), ( "settings" ) };

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
				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild( ( "configuration" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::EndChild( );
			}
			break;
		}
		case 1: /* visuals */ {
			if ( ImGui::BeginChild(
					 ( "esp" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ),
					 true, 0, true ) ) {
				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild(
					 ( "chams" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ),
					 true, 0, true ) ) {
				static int selected_chams_layer = 0;
				ImGui::Combo( "chams layer selection", &selected_chams_layer, "layer one\0layer two\0layer three\0layer four\0\0" );

				ImGui::EndChild( );
			}

			if ( ImGui::BeginChild(
					 ( "glow" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ), true,
					 0, true ) ) {
				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			static int other_subtab_number = 0;
			if ( ImGui::BeginChild( ( "other" ), { "main", "effects", "removals" }, &other_subtab_number,
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {

				switch ( other_subtab_number ) {
				case 0: {
					ImGui::Checkbox( "precipitation", &GET_VARIABLE( g_variables.m_precipitation, bool ) );
					if ( GET_VARIABLE( g_variables.m_precipitation, bool ) ) {
						/*ImGui::ColorEdit4( "##precipitation color", &GET_VARIABLE( g_variables.m_precipitation_color, c_color ),
						                   color_picker_alpha_flags );*/

						ImGui::Combo( "weather type", &GET_VARIABLE( g_variables.m_precipitation_type, int ), "rain\0ash\0rain storm\0snow" );
					}

					ImGui::Checkbox( "fog", &GET_VARIABLE( g_variables.m_fog, bool ) );
					if ( GET_VARIABLE( g_variables.m_fog, bool ) ) {
						ImGui::ColorEdit4( "##fog picker", &GET_VARIABLE( g_variables.m_fog_color, c_color ), color_picker_alpha_flags );

						ImGui::SliderFloat( "start##fog", &GET_VARIABLE( g_variables.m_fog_start, float ), 0.f, 5000.f, "%.1f" );
						ImGui::SliderFloat( "end##fog", &GET_VARIABLE( g_variables.m_fog_end, float ), 0.f, 5000.f, "%.1f" );
					}
					break;
				}
				case 1: {
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
				case 2: {
					ImGui::Checkbox( "disable post processing", &GET_VARIABLE( g_variables.m_disable_post_processing, bool ) );
					ImGui::Checkbox( "remove panorama blur", &GET_VARIABLE( g_variables.m_remove_panorama_blur, bool ) );
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

				ImGui::Checkbox( "long jump", &GET_VARIABLE( g_variables.m_long_jump, bool ) );
				ImGui::Keybind( "long jump key", &GET_VARIABLE( g_variables.m_long_jump_key, key_bind_t ) );

				ImGui::Checkbox( "mini jump", &GET_VARIABLE( g_variables.m_mini_jump, bool ) );
				ImGui::Keybind( "mini jump key", &GET_VARIABLE( g_variables.m_mini_jump_key, key_bind_t ) );

				ImGui::Checkbox( "jump bug", &GET_VARIABLE( g_variables.m_jump_bug, bool ) );
				ImGui::Keybind( "jump bug key", &GET_VARIABLE( g_variables.m_jump_bug_key, key_bind_t ) );

				ImGui::Checkbox( "edge bug", &GET_VARIABLE( g_variables.m_edge_bug, bool ) );
				ImGui::Keybind( "edge bug key", &GET_VARIABLE( g_variables.m_edge_bug_key, key_bind_t ) );

				if ( GET_VARIABLE( g_variables.m_edge_bug, bool ) ) {
					ImGui::SliderInt( "edge bug predicted ticks", &GET_VARIABLE( g_variables.m_edge_bug_ticks, int ), 0, 32 );
					ImGui::Checkbox( "advanced detection", &GET_VARIABLE( g_variables.m_advanced_detection, bool ) );
					ImGui::SliderFloat( "mouse lock scale", &GET_VARIABLE( g_variables.m_edge_bug_lock_amt, float ), 0.f, 1.f );
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
					if ( GET_VARIABLE( g_variables.m_velocity_indicator_custom_color, bool ) ) {
						ImGui::ColorEdit4( "##velocity indicator color 1", &GET_VARIABLE( g_variables.m_velocity_indicator_color1, c_color ),
						                   color_picker_alpha_flags );

						ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

						ImGui::ColorEdit4( "##velocity indicator color 2", &GET_VARIABLE( g_variables.m_velocity_indicator_color2, c_color ),
						                   color_picker_alpha_flags );
					} else {
						ImGui::ColorEdit4( "##velocity indicator color 3", &GET_VARIABLE( g_variables.m_velocity_indicator_color3, c_color ),
						                   color_picker_alpha_flags );

						ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

						ImGui::ColorEdit4( "##velocity indicator color 4", &GET_VARIABLE( g_variables.m_velocity_indicator_color4, c_color ),
						                   color_picker_alpha_flags );

						ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 50.f );

						ImGui::ColorEdit4( "##velocity indicator color 5", &GET_VARIABLE( g_variables.m_velocity_indicator_color5, c_color ),
						                   color_picker_alpha_flags );
					}

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "show pre speed##velocity indicator", &GET_VARIABLE( g_variables.m_velocity_indicator_show_pre_speed, bool ) );

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "fade alpha##velocity indicator", &GET_VARIABLE( g_variables.m_velocity_indicator_fade_alpha, bool ) );

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "custom color##velocity indicator", &GET_VARIABLE( g_variables.m_velocity_indicator_custom_color, bool ) );

					ImGui::SliderInt( "padding##velocity indicator", &GET_VARIABLE( g_variables.m_velocity_indicator_padding, int ), 5, 100, "%d%%" );
				}

				ImGui::Checkbox( "stamina indicator", &GET_VARIABLE( g_variables.m_stamina_indicator, bool ) );
				if ( GET_VARIABLE( g_variables.m_stamina_indicator, bool ) ) {
					ImGui::ColorEdit4( "##stamina indicator color 1", &GET_VARIABLE( g_variables.m_stamina_indicator_color1, c_color ),
					                   color_picker_alpha_flags );

					ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

					ImGui::ColorEdit4( "##stamina indicator color 2", &GET_VARIABLE( g_variables.m_stamina_indicator_color2, c_color ),
					                   color_picker_alpha_flags );

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "show pre speed##stamina indicator", &GET_VARIABLE( g_variables.m_stamina_indicator_show_pre_speed, bool ) );

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "fade alpha##stamina indicator", &GET_VARIABLE( g_variables.m_stamina_indicator_fade_alpha, bool ) );

					ImGui::SliderInt( "padding##stamina indicator", &GET_VARIABLE( g_variables.m_stamina_indicator_padding, int ), 5, 100, "%d%%" );
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
		case 5: /* settings */
		{
			if ( ImGui::BeginChild( ( "settings" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::InputText( ( "config file name" ), this->m_config_file, sizeof( this->m_config_file ) );

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

					converted_file_name.clear( );
					g_config.refresh( );
				}

				if ( ImGui::Button( ( "save" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					if ( !g_config.save( selected_config_name ) )
						g_console.print( std::vformat( "failed to save {:s}", std::make_format_args( selected_config_name ) ).c_str( ) );

				if ( ImGui::Button( ( "load" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					if ( !g_config.load( selected_config_name ) )
						g_console.print( std::vformat( "failed to load {:s}", std::make_format_args( converted_file_name ) ).c_str( ) );

					// memory.m_client_state->m_delta_tick = -1;
				}

				if ( ImGui::Button( ( "remove" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					g_config.remove( this->m_selected_config );
					this->m_selected_config = 0;
				}

				if ( ImGui::Button( ( "refresh" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					g_config.refresh( );

				ImGui::SetCursorPosX( ImGui::GetCursorPos( ).x + 17.f );
				ImGui::Text( "accent color" );

				ImGui::ColorEdit4( ( "##accent color" ), &GET_VARIABLE( g_variables.m_accent, c_color ), color_picker_no_alpha_flags );
				ImGui::EndChild( );
			}
			break;
		}
		}
	}
	ImGui::End( );
}
