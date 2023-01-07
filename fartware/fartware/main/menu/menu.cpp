#include "menu.h"

#include "../includes.h"

constexpr int color_picker_alpha_flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf |
                                         ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoInputs |
                                         ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar |
                                         ImGuiColorEditFlags_NoBorder;

constexpr int color_picker_no_alpha_flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_InputRGB |
                                            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop |
                                            ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder;

/* TODO ~ float ~ add popup system (on right click) render settings popup (render keybinder last) */

/* TODO ~ float ~ XORstr :4 */
void menu_t::on_end_scene( )
{
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

			/* render gradient */
			RenderFadedGradientLine( draw_list, ImVec2( position.x, position.y + background_height - 1.f ), ImVec2( size.x, 1.f ),
			                         ImColor( Accent[ 0 ], Accent[ 1 ], Accent[ 2 ] ) );

			const auto title_text      = xs( "hotwheels" );
			const auto title_text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
				render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, title_text );

			const auto vip_title_text      = xs( ".vip" );
			const auto vip_title_text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
				render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, vip_title_text );

			draw_list->AddText( render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
			                    render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
			                    ImVec2( position.x + ( ( size.x - title_text_size.x - vip_title_text_size.x ) / 2.f ),
			                            position.y + ( ( background_height - title_text_size.y ) / 2.f ) ),
			                    ImColor( 1.f, 1.f, 1.f ), title_text );

			draw_list->AddText( render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
			                    render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
			                    ImVec2( position.x + ( ( size.x + title_text_size.x - vip_title_text_size.x ) / 2.f ),
			                            position.y + ( ( background_height - title_text_size.y ) / 2.f ) ),
			                    ImColor( Accent[ 0 ], Accent[ 1 ], Accent[ 2 ] ), vip_title_text );
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
			                         ImColor( Accent[ 0 ], Accent[ 1 ], Accent[ 2 ] ) );

			std::vector< const char* > tab_names = {
				xs( "aimbot" ), xs( "visuals" ), xs( "movement" ), xs( "misc" ), xs( "skins" ), xs( "settings" )
			};

			/* tab logic */
			for ( int iterator = { }; iterator < static_cast< int >( tab_names.size( ) ); iterator++ ) {
				if ( !( iterator < static_cast< int >( tab_names.size( ) ) ) )
					break;

				const char* const tab_name = tab_names[ iterator ];

				const auto hashed_tab_name = ImHashStr( tab_name );

				const auto text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, tab_name );

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

				draw_list->AddText( render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
				                    render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
				                    ImVec2( position.x + text_position.x, position.y + text_position.y ),
				                    ImColor::Blend( ImColor( 1.f, 1.f, 1.f, hovered_text_animation.AnimationData->second ),
				                                    ImColor( Accent[ 0 ], Accent[ 1 ], Accent[ 2 ] ), selected_animation.AnimationData->second ),
				                    tab_name );

				if ( hovered && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) )
					tab_number = iterator;
			}
		}( );

		/* set next working position for the elements */
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 25.f );

		switch ( tab_number ) {
		case 1: /* visuals */ {
			if ( ImGui::BeginChild(
					 xs( "esp" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ), true,
					 0, true ) ) {
				ImGui::Checkbox( xs( "bounding box" ), &GET_CONFIG_BOOL( variables.m_visuals.m_bounding_box ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_bounding_box ) ) {
					ImGui::ColorEdit4( xs( "##bounding box color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_bounding_box_color ),
					                   color_picker_alpha_flags );
					ImGui::Checkbox( xs( "outline##bounding box" ), &GET_CONFIG_BOOL( variables.m_visuals.m_bounding_box_outline ) );
				}

				ImGui::Checkbox( xs( "player name" ), &GET_CONFIG_BOOL( variables.m_visuals.m_player_name ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_player_name ) ) {
					ImGui::ColorEdit4( xs( "##player name color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_player_name_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::Checkbox( xs( "player health bar" ), &GET_CONFIG_BOOL( variables.m_visuals.m_player_health_bar ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_player_health_bar ) ) {
					ImGui::Checkbox( xs( "custom color##player health bar" ), &GET_CONFIG_BOOL( variables.m_visuals.m_custom_color_health_bar ) );

					if ( GET_CONFIG_BOOL( variables.m_visuals.m_custom_color_health_bar ) )
						ImGui::ColorEdit4( xs( "##player health bar color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_player_health_bar_color ),
						                   color_picker_alpha_flags );
				}

				ImGui::MultiCombo( xs( "player flags" ), config.get< std::vector< bool > >( variables.m_visuals.m_player_flags ),
				                   { xs( "money" ), xs( "armor" ) },
				                   config.get< std::vector< bool > >( variables.m_visuals.m_player_flags ).size( ) );

				ImGui::Checkbox( xs( "active weapon name" ), &GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_name ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_name ) ) {
					ImGui::ColorEdit4( xs( "##active weapon name color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_active_weapon_name_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::Checkbox( xs( "active weapon icon" ), &GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_icon ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_icon ) ) {
					ImGui::ColorEdit4( xs( "##active weapon icon color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_active_weapon_icon_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::Checkbox( xs( "weapon ammo bar" ), &GET_CONFIG_BOOL( variables.m_visuals.m_weapon_ammo_bar ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_weapon_ammo_bar ) ) {
					ImGui::ColorEdit4( xs( "##weapon ammo bar color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_weapon_ammo_bar_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::Checkbox( xs( "player avatar" ), &GET_CONFIG_BOOL( variables.m_visuals.m_player_avatar ) );
				/*if ( GET_CONFIG_BOOL( variables.m_visuals.m_player_avatar ) ) {
				    ImGui::ColorEdit4( "##player avatar color", &GET_CONFIG_COLOR( variables.m_visuals.m_player_avatar_color ),
				                       color_picker_alpha_flags );
				}*/

				ImGui::Checkbox( xs( "out of fov arrows" ), &GET_CONFIG_BOOL( variables.m_visuals.m_out_of_fov_arrows ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_out_of_fov_arrows ) ) {
					ImGui::ColorEdit4( xs( "##out of fov arrows color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_out_of_fov_arrows_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild(
					 xs( "glow" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ), true, 0,
					 true ) ) {
				ImGui::EndChild( );
			}

			if ( ImGui::BeginChild( xs( "chams" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::EndChild( );
			}
			break;
		}
		case 2: /* misc */ {
			if ( ImGui::BeginChild(
					 xs( "movement" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ), true,
					 0, true ) ) {

				ImGui::Checkbox( "bunny hop", &GET_CONFIG_BOOL( variables.m_movement.m_bunny_hop ) );

				ImGui::Checkbox( "edge jump", &GET_CONFIG_BOOL( variables.m_movement.m_edge_jump ) );
				if ( GET_CONFIG_BOOL( variables.m_movement.m_edge_jump ) )
					ImGui::Keybind( "edge jump key", &GET_CONFIG_BIND( variables.m_movement.m_edge_jump_key ) );

				ImGui::Checkbox( "long jump", &GET_CONFIG_BOOL( variables.m_movement.m_long_jump ) );
				if ( GET_CONFIG_BOOL( variables.m_movement.m_long_jump ) )
					ImGui::Keybind( "long jump key", &GET_CONFIG_BIND( variables.m_movement.m_long_jump_key ) );

				ImGui::Checkbox( "mini jump", &GET_CONFIG_BOOL( variables.m_movement.m_mini_jump ) );
				if ( GET_CONFIG_BOOL( variables.m_movement.m_mini_jump ) )
					ImGui::Keybind( "mini jump key", &GET_CONFIG_BIND( variables.m_movement.m_mini_jump_key ) );

				ImGui::Checkbox( "jump bug", &GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) );
				if ( GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) )
					ImGui::Keybind( "jump bug key", &GET_CONFIG_BIND( variables.m_movement.m_jump_bug_key ) );

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild(
					 xs( "indicators" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ), true, 0,
					 true ) ) {
				ImGui::EndChild( );
			}

			if ( ImGui::BeginChild( xs( "game" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild( xs( "exploits" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::EndChild( );
			}
			break;
		}
		case 5: /* settings */
		{
			if ( ImGui::BeginChild( xs( "settings" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::InputText( xs( "config file name" ), this->m_config_file, sizeof( this->m_config_file ) );

				std::string converted_file_name = this->m_config_file;

				static std::string selected_config_name = { };

				ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail( ).x - 2.f );

				ImGui::PushStyleVar( ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2( 0.f, 3.f ) );

				ImGui::ListBox(
					xs( "##config list" ), &this->m_selected_config, []( int index ) { return config.m_file_names[ index ].c_str( ); },
					config.m_file_names.size( ), 5 );

				ImGui::PopStyleVar( );
				selected_config_name = !config.m_file_names.empty( ) ? config.m_file_names[ this->m_selected_config ] : "";

				if ( ImGui::Button( xs( "create" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					if ( !config.save( converted_file_name ) )
						console.print( "failed to create {:s}", converted_file_name );

					converted_file_name.clear( );
					config.refresh( );
				}

				if ( ImGui::Button( xs( "save" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					if ( !config.save( selected_config_name ) )
						console.print( "failed to save {:s}", selected_config_name );

				if ( ImGui::Button( xs( "load" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					if ( !config.load( selected_config_name ) )
						console.print( "failed to load {:s}", converted_file_name );

				if ( ImGui::Button( xs( "remove" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					config.remove( this->m_selected_config );
					this->m_selected_config = 0;
				}

				if ( ImGui::Button( xs( "refresh" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					config.refresh( );

				ImGui::EndChild( );
			}
			break;
		}
		}
	}
	ImGui::End( );
}