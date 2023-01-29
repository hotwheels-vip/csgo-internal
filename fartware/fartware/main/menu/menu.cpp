#include "menu.h"

#include "../includes.h"

#include "../logging/logging.h"

constexpr int color_picker_alpha_flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf |
                                         ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoInputs |
                                         ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar |
                                         ImGuiColorEditFlags_NoBorder;

constexpr int color_picker_no_alpha_flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_InputRGB |
                                            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop |
                                            ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder;

void menu_t::on_end_scene( )
{
	ImGui::GetStyle( ).Colors[ ImGuiCol_::ImGuiCol_Accent ] = GET_CONFIG_COLOR( variables.m_menu.m_accent ).get_vec4( );

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
			const auto title_text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
				render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, title_text );

			const auto vip_title_text      = ( ".vip" );
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
				                                    ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent ), selected_animation.AnimationData->second ),
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
					 ( "esp" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ), true,
					 0, true ) ) {
				ImGui::Checkbox( ( "bounding box" ), &GET_CONFIG_BOOL( variables.m_visuals.m_bounding_box ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_bounding_box ) ) {
					ImGui::ColorEdit4( ( "##bounding box color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_bounding_box_color ),
					                   color_picker_alpha_flags );
					ImGui::Checkbox( ( "outline##bounding box" ), &GET_CONFIG_BOOL( variables.m_visuals.m_bounding_box_outline ) );
				}

				ImGui::Checkbox( ( "player name" ), &GET_CONFIG_BOOL( variables.m_visuals.m_player_name ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_player_name ) ) {
					ImGui::ColorEdit4( ( "##player name color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_player_name_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::Checkbox( ( "player health bar" ), &GET_CONFIG_BOOL( variables.m_visuals.m_player_health_bar ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_player_health_bar ) ) {
					ImGui::Checkbox( ( "custom color##player health bar" ), &GET_CONFIG_BOOL( variables.m_visuals.m_custom_color_health_bar ) );

					if ( GET_CONFIG_BOOL( variables.m_visuals.m_custom_color_health_bar ) )
						ImGui::ColorEdit4( ( "##player health bar color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_player_health_bar_color ),
						                   color_picker_alpha_flags );
				}

				ImGui::MultiCombo( "player flags", config.get< std::vector< bool > >( variables.m_visuals.m_player_flags ),
				                   { "money", "armor", "reload", "bomb" },
				                   config.get< std::vector< bool > >( variables.m_visuals.m_player_flags ).size( ) );

				ImGui::Checkbox( ( "active weapon name" ), &GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_name ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_name ) ) {
					ImGui::ColorEdit4( ( "##active weapon name color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_active_weapon_name_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::Checkbox( ( "active weapon icon" ), &GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_icon ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_icon ) ) {
					ImGui::ColorEdit4( ( "##active weapon icon color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_active_weapon_icon_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::Checkbox( ( "weapon ammo bar" ), &GET_CONFIG_BOOL( variables.m_visuals.m_weapon_ammo_bar ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_weapon_ammo_bar ) ) {
					ImGui::ColorEdit4( ( "##weapon ammo bar color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_weapon_ammo_bar_color ),
					                   color_picker_alpha_flags );
				}

				ImGui::Checkbox( ( "player avatar" ), &GET_CONFIG_BOOL( variables.m_visuals.m_player_avatar ) );
				/*if ( GET_CONFIG_BOOL( variables.m_visuals.m_player_avatar ) ) {
				    ImGui::ColorEdit4( "##player avatar color", &GET_CONFIG_COLOR( variables.m_visuals.m_player_avatar_color ),
				                       color_picker_alpha_flags );
				}*/

				ImGui::Checkbox( ( "out of fov arrows" ), &GET_CONFIG_BOOL( variables.m_visuals.m_out_of_fov_arrows ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_out_of_fov_arrows ) ) {
					ImGui::ColorEdit4( ( "##out of fov arrows color" ), &GET_CONFIG_COLOR( variables.m_visuals.m_out_of_fov_arrows_color ),
					                   color_picker_alpha_flags );
					ImGui::SliderFloat( "arrows size", &GET_CONFIG_FLOAT( variables.m_visuals.m_out_of_fov_arrows_size ), 0.1f, 50.f, "%.1f px" );
					ImGui::SliderInt( "arrows distance", &GET_CONFIG_INT( variables.m_visuals.m_out_of_fov_arrows_distance ), 10, 500, "%d px" );
				}

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild(
					 ( "chams" ), ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y / 2.f ) - background_height - 20.f ),
					 true, 0, true ) ) {
				static int selected_chams_layer = 0;
				ImGui::Combo( "chams layer selection", &selected_chams_layer, "layer one\0layer two\0layer three\0layer four\0\0" );

				switch ( selected_chams_layer ) {
				case 0: /* first layer */ {
					ImGui::Checkbox( "visible##first layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_enable_visible );
					ImGui::ColorEdit4( "##visible first layer color", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_visible_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_enable_visible )
						ImGui::Combo( "material##visible first layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_visible_material,
						              base_material_names, IM_ARRAYSIZE( base_material_names ) );

					ImGui::Checkbox( "invisible##first layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_enable_invisible );
					ImGui::ColorEdit4( "##invisible first layer color", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_invisible_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_enable_invisible )
						ImGui::Combo( "material##invisible first layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_invisible_material, base_material_names,
						              IM_ARRAYSIZE( base_material_names ) );

					ImGui::Checkbox( "visible overlay##first layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_enable_visible_overlay );
					ImGui::ColorEdit4( "##visible overlay first layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_visible_overlay_color, color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_enable_visible_overlay )
						ImGui::Combo( "material##visible overlay first layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_visible_overlay_material, overlay_material_names,
						              IM_ARRAYSIZE( overlay_material_names ) );

					ImGui::Checkbox( "invisible overlay##first layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_enable_invisible_overlay );
					ImGui::ColorEdit4( "##invisible overlay first layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_invisible_overlay_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_enable_invisible_overlay )
						ImGui::Combo( "material##invisible overlay first layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_invisible_overlay_material, overlay_material_names,
						              IM_ARRAYSIZE( overlay_material_names ) );

					ImGui::Checkbox( "render og model##first layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ).m_render_original_model );
					break;
				}
				case 1: /* second layer */ {
					ImGui::Checkbox( "visible##second layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_enable_visible );
					ImGui::ColorEdit4( "##visible second layer color", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_visible_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_enable_visible )
						ImGui::Combo( "material##visible second layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_visible_material,
						              base_material_names, IM_ARRAYSIZE( base_material_names ) );

					ImGui::Checkbox( "invisible##second layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_enable_invisible );
					ImGui::ColorEdit4( "##invisible second layer color", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_invisible_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_enable_invisible )
						ImGui::Combo( "material##invisible second layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_invisible_material, base_material_names,
						              IM_ARRAYSIZE( base_material_names ) );

					ImGui::Checkbox( "visible overlay##second layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_enable_visible_overlay );
					ImGui::ColorEdit4( "##visible overlay second layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_visible_overlay_color, color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_enable_visible_overlay )
						ImGui::Combo( "material##visible overlay second layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_visible_overlay_material, overlay_material_names,
						              IM_ARRAYSIZE( overlay_material_names ) );

					ImGui::Checkbox( "invisible overlay##second layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_enable_invisible_overlay );
					ImGui::ColorEdit4( "##invisible overlay second layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_invisible_overlay_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_enable_invisible_overlay )
						ImGui::Combo( "material##invisible overlay second layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_invisible_overlay_material, overlay_material_names,
						              IM_ARRAYSIZE( overlay_material_names ) );

					ImGui::Checkbox( "render og model##second layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ).m_render_original_model );
					break;
				}
				case 2: /* third layer */ {
					ImGui::Checkbox( "visible##third layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_enable_visible );
					ImGui::ColorEdit4( "##visible third layer color", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_visible_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_enable_visible )
						ImGui::Combo( "material##visible third layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_visible_material, base_material_names,
						              IM_ARRAYSIZE( base_material_names ) );

					ImGui::Checkbox( "invisible##third layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_enable_invisible );
					ImGui::ColorEdit4( "##invisible third layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_invisible_color, color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_enable_invisible )
						ImGui::Combo( "material##invisible third layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_invisible_material, base_material_names,
						              IM_ARRAYSIZE( base_material_names ) );

					ImGui::Checkbox( "visible overlay##third layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_enable_visible_overlay );
					ImGui::ColorEdit4( "##visible overlay third layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_visible_overlay_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_enable_visible_overlay )
						ImGui::Combo( "material##visible overlay third layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_visible_overlay_material, overlay_material_names,
						              IM_ARRAYSIZE( overlay_material_names ) );

					ImGui::Checkbox( "invisible overlay##third layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_enable_invisible_overlay );
					ImGui::ColorEdit4( "##invisible overlay third layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_invisible_overlay_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_enable_invisible_overlay )
						ImGui::Combo( "material##invisible overlay third layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_invisible_overlay_material,
						              overlay_material_names, IM_ARRAYSIZE( overlay_material_names ) );

					ImGui::Checkbox( "render og model##third layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ).m_render_original_model );
					break;
				}
				case 3: /* fourth layer */ {
					ImGui::Checkbox( "visible##fourth layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_enable_visible );
					ImGui::ColorEdit4( "##visible fourth layer color", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_visible_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_enable_visible )
						ImGui::Combo( "material##visible fourth layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_visible_material, base_material_names,
						              IM_ARRAYSIZE( base_material_names ) );

					ImGui::Checkbox( "invisible##fourth layer", &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_enable_invisible );
					ImGui::ColorEdit4( "##invisible fourth layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_invisible_color, color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_enable_invisible )
						ImGui::Combo( "material##invisible fourth layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_invisible_material, base_material_names,
						              IM_ARRAYSIZE( base_material_names ) );

					ImGui::Checkbox( "visible overlay##fourth layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_enable_visible_overlay );
					ImGui::ColorEdit4( "##visible overlay fourth layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_visible_overlay_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_enable_visible_overlay )
						ImGui::Combo( "material##visible overlay fourth layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_visible_overlay_material, overlay_material_names,
						              IM_ARRAYSIZE( overlay_material_names ) );

					ImGui::Checkbox( "invisible overlay##fourth layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_enable_invisible_overlay );
					ImGui::ColorEdit4( "##invisible overlay fourth layer color",
					                   &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_invisible_overlay_color,
					                   color_picker_alpha_flags );

					if ( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_enable_invisible_overlay )
						ImGui::Combo( "material##invisible overlay fourth layer",
						              &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_invisible_overlay_material,
						              overlay_material_names, IM_ARRAYSIZE( overlay_material_names ) );

					ImGui::Checkbox( "render og model##fourth layer",
					                 &GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ).m_render_original_model );
					break;
				}
				}

				ImGui::EndChild( );
			}

			if ( ImGui::BeginChild(
					 ( "glow" ), ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
					 true, 0, true ) ) {
				// TODO add vis and invis colors
				ImGui::Checkbox( "player glow", &GET_CONFIG_BOOL( variables.m_visuals.m_glow_enable ) );
				ImGui::ColorEdit4( "##player glow color", &GET_CONFIG_COLOR( variables.m_visuals.m_glow_color ), color_picker_alpha_flags );
				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild( ( "world" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::Checkbox( "enable world fog", &GET_CONFIG_BOOL( variables.m_world.m_fog_enable ) );
				if ( GET_CONFIG_BOOL( variables.m_world.m_fog_enable ) ) {
					ImGui::ColorEdit4( "##fog color", &GET_CONFIG_COLOR( variables.m_world.m_fog_color ), color_picker_no_alpha_flags );
					ImGui::SliderFloat( "fog start", &GET_CONFIG_FLOAT( variables.m_world.m_fog_start ), 0.f, 5000.f, "%.1f" );
					ImGui::SliderFloat( "fog end", &GET_CONFIG_FLOAT( variables.m_world.m_fog_end ), 0.f, 5000.f, "%.1f" );
					ImGui::SliderFloat( "fog density", &GET_CONFIG_FLOAT( variables.m_world.m_fog_density ), 0.f, 1.f, "%.2f" );
				};
				ImGui::EndChild( );
			}
			break;
		}
		case 2: /* movement */ {
			if ( ImGui::BeginChild(
					 ( "movement" ),
					 ImVec2( ImGui::GetContentRegionAvail( ).x / 2.f, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ), true, 0,
					 true ) ) {
				ImGui::Checkbox( "bunny hop", &GET_CONFIG_BOOL( variables.m_movement.m_bunny_hop ) );

				ImGui::Checkbox( "edge bug", &GET_CONFIG_BOOL( variables.m_movement.m_edge_bug ) );
				ImGui::Keybind( "edge bug key", &GET_CONFIG_BIND( variables.m_movement.m_edge_bug_key ) );

				if ( GET_CONFIG_BOOL( variables.m_movement.m_edge_bug ) ) {
					ImGui::SliderInt( "edge bug predicted ticks", &GET_CONFIG_INT( variables.m_movement.m_edge_bug_ticks ), 0, 32 );
					ImGui::Checkbox( "advanced detection", &GET_CONFIG_BOOL( variables.m_movement.m_advanced_detection ) );
					ImGui::SliderFloat( "mouse lock scale", &GET_CONFIG_FLOAT( variables.m_movement.m_edge_bug_lock_amt ), 0.f, 1.f );
				}

				ImGui::Checkbox( "edge jump", &GET_CONFIG_BOOL( variables.m_movement.m_edge_jump ) );
				ImGui::Keybind( "edge jump key", &GET_CONFIG_BIND( variables.m_movement.m_edge_jump_key ) );

				ImGui::Checkbox( "long jump", &GET_CONFIG_BOOL( variables.m_movement.m_long_jump ) );
				ImGui::Keybind( "long jump key", &GET_CONFIG_BIND( variables.m_movement.m_long_jump_key ) );

				ImGui::Checkbox( "mini jump", &GET_CONFIG_BOOL( variables.m_movement.m_mini_jump ) );
				ImGui::Keybind( "mini jump key", &GET_CONFIG_BIND( variables.m_movement.m_mini_jump_key ) );

				ImGui::Checkbox( "jump bug", &GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) );
				ImGui::Keybind( "jump bug key", &GET_CONFIG_BIND( variables.m_movement.m_jump_bug_key ) );

				ImGui::Checkbox( "pixel surf", &GET_CONFIG_BOOL( variables.m_movement.m_pixel_surf ) );
				ImGui::Keybind( "pixel surf key", &GET_CONFIG_BIND( variables.m_movement.m_pixel_surf_key ) );

				ImGui::Checkbox( "delay hop", &GET_CONFIG_BOOL( variables.m_movement.m_delay_hop ) );
				ImGui::Keybind( "delay hop key", &GET_CONFIG_BIND( variables.m_movement.m_delay_hop_key ) );

				ImGui::Checkbox( "autostrafe", &GET_CONFIG_BOOL( variables.m_movement.m_autostrafe ) );
				ImGui::Keybind( "autostrafe key", &GET_CONFIG_BIND( variables.m_movement.m_autostrafe_key ) );

				ImGui::Checkbox( "no crouch cooldown", &GET_CONFIG_BOOL( variables.m_movement.m_fast_duck ) );

				ImGui::Checkbox( "auto duck", &GET_CONFIG_BOOL( variables.m_movement.m_autoduck ) );

				ImGui::Checkbox( "auto align", &GET_CONFIG_BOOL( variables.m_movement.m_auto_align ) );

				ImGui::EndChild( );
			}

			ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 20.f );

			if ( ImGui::BeginChild( ( "indicators" ),
			                        ImVec2( ImGui::GetContentRegionAvail( ).x, ( ImGui::GetContentRegionAvail( ).y ) - background_height - 20.f ),
			                        true, 0, true ) ) {
				ImGui::Checkbox( "velocity indicator", &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator ) );
				if ( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator ) ) {
					if ( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_custom_color ) ) {
						ImGui::ColorEdit4( "##velocity indicator color 1",
						                   &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color1 ),
						                   color_picker_alpha_flags );

						ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

						ImGui::ColorEdit4( "##velocity indicator color 2",
						                   &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color2 ),
						                   color_picker_alpha_flags );
					} else {
						ImGui::ColorEdit4( "##velocity indicator color 3",
						                   &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color3 ),
						                   color_picker_alpha_flags );

						ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

						ImGui::ColorEdit4( "##velocity indicator color 4",
						                   &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color4 ),
						                   color_picker_alpha_flags );

						ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 50.f );

						ImGui::ColorEdit4( "##velocity indicator color 5",
						                   &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color5 ),
						                   color_picker_alpha_flags );
					}

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "show pre speed##velocity indicator",
					                 &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_show_pre_speed ) );

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "fade alpha##velocity indicator",
					                 &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_fade_alpha ) );

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "custom color##velocity indicator",
					                 &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_custom_color ) );

					ImGui::SliderInt( "position##velocity indicator",
					                  &GET_CONFIG_INT( variables.m_movement.m_indicators.m_velocity_indicator_position ), 30,
					                  globals.m_display_size.y );
				}
				ImGui::Checkbox( "velocity graph", &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_graph ) );
				if ( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_graph ) )
					ImGui::SliderInt( "position##velocity graph", &GET_CONFIG_INT( variables.m_movement.m_indicators.m_velocity_graph_position ), 30,
					                  globals.m_display_size.y );

				ImGui::Checkbox( "stamina indicator", &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_stamina_indicator ) );
				if ( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_stamina_indicator ) ) {
					ImGui::ColorEdit4( "##stamina indicator color 1",
					                   &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_stamina_indicator_color1 ), color_picker_alpha_flags );

					ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

					ImGui::ColorEdit4( "##stamina indicator color 2",
					                   &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_stamina_indicator_color2 ), color_picker_alpha_flags );

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "show pre speed##stamina indicator",
					                 &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_stamina_indicator_show_pre_speed ) );

					ImGui::SetCursorPosX( 26.f );
					ImGui::Checkbox( "fade alpha##stamina indicator",
					                 &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_stamina_indicator_fade_alpha ) );

					ImGui::SliderInt( "position##stamina indicator",
					                  &GET_CONFIG_INT( variables.m_movement.m_indicators.m_stamina_indicator_position ), 30,
					                  globals.m_display_size.y );
				}

				ImGui::Checkbox( "keybind indicators", &GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_sub_indicators_enable ) );
				if ( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_sub_indicators_enable ) ) {
					ImGui::ColorEdit4( "##keybind color", &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color ),
					                   color_picker_alpha_flags );

					ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

					ImGui::ColorEdit4( "##keybind success color", &GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color_success ),
					                   color_picker_alpha_flags );

					ImGui::SliderInt( "position##sub indicators", &GET_CONFIG_INT( variables.m_movement.m_indicators.m_sub_indicators_position ), 30,
					                  globals.m_display_size.y );

					ImGui::MultiCombo( "displayed keybinds", config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators ),
					                   { "edgebug", "pixelsurf", "edgejump", "longjump", "delayhop", "minijump", "jumpbug" },
					                   config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators ).size( ) );
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
				// SPECTATOR LIST SHOULD BE HERE. FUCK YOU FLOAT

				ImGui::Checkbox( "spectator list", &GET_CONFIG_BOOL( variables.m_visuals.m_spectators_list ) );
				if ( GET_CONFIG_BOOL( variables.m_visuals.m_spectators_list ) ) {
					ImGui::ColorEdit4( ( "##spectator list text color one" ),
					                   &GET_CONFIG_COLOR( variables.m_visuals.m_spectators_list_text_color_one ), color_picker_alpha_flags );

					ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 25.f );

					ImGui::ColorEdit4( ( "##spectator list text color two" ),
					                   &GET_CONFIG_COLOR( variables.m_visuals.m_spectators_list_text_color_two ), color_picker_alpha_flags );
				}

				ImGui::Checkbox( "practice window", &GET_CONFIG_BOOL( variables.m_misc.m_practice_window ) );
				if ( GET_CONFIG_BOOL( variables.m_misc.m_practice_window ) ) {
					ImGui::Text( "practice checkpoint key" );
					ImGui::Keybind( "practice checkpoint key", &GET_CONFIG_BIND( variables.m_misc.m_practice_cp_key ) );
					ImGui::Text( "practice teleport key" );
					ImGui::Keybind( "practice teleport key", &GET_CONFIG_BIND( variables.m_misc.m_practice_tp_key ) );
				}

				// TODO: combobox between force crosshair and just draw a white dot
				ImGui::Checkbox( "sniper crosshair", &GET_CONFIG_BOOL( variables.m_visuals.m_sniper_crosshair ) );
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

				ImGui::ListBox( ( "##config list" ), &this->m_selected_config, []( int index ) { return config.m_file_names[ index ].c_str( ); },
				                config.m_file_names.size( ), 5 );

				ImGui::PopStyleVar( );
				selected_config_name = !config.m_file_names.empty( ) ? config.m_file_names[ this->m_selected_config ] : "";

				if ( ImGui::Button( ( "create" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					if ( !config.save( converted_file_name ) )
						console.print( "failed to create {:s}", converted_file_name );

					converted_file_name.clear( );
					config.refresh( );
				}

				if ( ImGui::Button( ( "save" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					if ( !config.save( selected_config_name ) )
						console.print( "failed to save {:s}", selected_config_name );

				if ( ImGui::Button( ( "load" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					if ( !config.load( selected_config_name ) )
						console.print( "failed to load {:s}", converted_file_name );

					memory.m_client_state->m_delta_tick = -1;
				}

				if ( ImGui::Button( ( "remove" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) ) {
					config.remove( this->m_selected_config );
					this->m_selected_config = 0;
				}

				if ( ImGui::Button( ( "refresh" ), ImVec2( ImGui::GetContentRegionAvail( ).x - 33.f, 15.f ) ) )
					config.refresh( );

				ImGui::SetCursorPosX( ImGui::GetCursorPos( ).x + 17.f );
				ImGui::Text( "accent color" );

				ImGui::ColorEdit4( ( "##accent color" ), &GET_CONFIG_COLOR( variables.m_menu.m_accent ), color_picker_no_alpha_flags );
				ImGui::Checkbox( "watermark", &GET_CONFIG_BOOL( variables.m_menu.m_watermark ) );
				ImGui::EndChild( );
			}
			break;
		}
		}
	}
	ImGui::End( );
}
