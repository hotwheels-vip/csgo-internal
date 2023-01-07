#include "menu.h"
#include "../../hacks/logging/logging.h"

#include "../../dependencies/imgui/imgui.h"

#include "../../globals/interfaces/interfaces.h"

#include "../../utils/files/files.h"
#include "../../utils/keybinds/keybinds.h"

#include "imgui/imgui_helper.h"

bool menu::impl::init_input( )
{
	D3DDEVICE_CREATION_PARAMETERS creation_params = { };
	while ( FAILED( g_interfaces.device->GetCreationParameters( &creation_params ) ) )
		utils::sleep( 200 );

	window = creation_params.hFocusWindow;

	if ( !window )
		return false;

	return true;
}

void menu::impl::draw( )
{
	if ( !menu_open )
		return;

	g_imgui.set_next_window_size( ImVec2( 500, 550 ), ImGuiCond_Always );

	g_imgui.begin( _( "hotwheels-main-window" ), nullptr,
	               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar );
	{
		ImDrawList* window_draw_list = ImGui::GetWindowDrawList( );

		g_imgui.start_decorations( window_draw_list );

		g_imgui.spacing( 1 );

		g_imgui.begin_child( _( "tabs-child" ), ImVec2( -1, 30 ), true, 0 );
		{
			g_imgui.set_cursor_pos_x( 12 );
			g_imgui.render_tab( _( "aimbot" ), 0 );
			g_imgui.same_line( 0.f, 59.f );

			g_imgui.render_tab( _( "anti-aim" ), 1 );
			g_imgui.same_line( 0.f, 59.f );

			g_imgui.render_tab( _( "visuals" ), 2 );
			g_imgui.same_line( 0.f, 59.f );

			g_imgui.render_tab( _( "misc" ), 3 );
			g_imgui.same_line( 0.f, 59.f );

			g_imgui.render_tab( _( "skins" ), 4 );
			g_imgui.same_line( 0.f, 59.f );

			g_imgui.render_tab( _( "config" ), 5 );

			g_imgui.end_child( );
		}

		switch ( g_menu.menu_tab ) {
		case 0: { /* aimbot tab */
			g_imgui.begin_child( _( "aimbot-main-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x / 2, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator( window_draw_list,
				                   ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "general" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
				                   ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ),
				                   _( "general" ) );

				g_imgui.end_child( );
			}

			break;
		}
		case 1: { /* anti aim tab */
			g_imgui.begin_child( _( "anti-aim-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x / 2, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator( window_draw_list,
				                   ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "general" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
				                   ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ),
				                   _( "general" ) );
				//
				g_imgui.end_child( );
			}

			g_imgui.same_line( );

			g_imgui.begin_child( _( "exploits-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator(
					window_draw_list,
					ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "exploits" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
					ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ), _( "exploits" ) );

				g_imgui.spacing( );
				g_imgui.end_child( );
			}
			break;
		}
		case 2: { /* visuals tab */
			g_imgui.begin_child( _( "esp-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x / 2, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator(
					window_draw_list, ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "esp" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
					ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ), _( "esp" ) );

				g_imgui.end_child( );
			}

			g_imgui.same_line( );

			g_imgui.begin_child( _( "chams-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x, g_imgui.get_content_region_avail( ).y / 2 ) );
			{
				g_imgui.separator( window_draw_list,
				                   ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "chams" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
				                   ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ),
				                   _( "chams" ) );

				g_imgui.spacing( );
				g_imgui.end_child( );
			}

			g_imgui.set_cursor_pos( ImVec2( g_imgui.get_content_region_avail( ).x / 2 + 15, 258 ) );

			g_imgui.begin_child( _( "world-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator( window_draw_list,
				                   ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "world" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
				                   ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ),
				                   _( "world" ) );

				g_imgui.spacing( );
				g_imgui.end_child( );
			}
			break;
		}
		case 3: { /* misc tab */
			g_imgui.begin_child( _( "movement-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x / 2 - 8, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator(
					window_draw_list,
					ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "movement" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
					ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ), _( "movement" ) );

				g_imgui.checkbox( _( "auto bhop" ), &g_config.find< bool >( HASH_BT( "m_bh" ) ) );
				g_imgui.checkbox( _( "auto jumpbug" ), &g_config.find< bool >( HASH_BT( "m_jb" ) ) );
				g_imgui.keybind( _( "auto jumpbug key" ), &g_config.find< int >( HASH_BT( "m_jb_key" ) ), 15 );
				g_imgui.checkbox( _( "edge jump" ), &g_config.find< bool >( HASH_BT( "m_ej" ) ) );
				g_imgui.keybind( _( "edge jump key" ), &g_config.find< int >( HASH_BT( "m_ej_key" ) ), 15 );
				if ( g_config.find< bool >( HASH_BT( "m_ej" ) ) )
					g_imgui.checkbox( _( "long jump" ), &g_config.find< bool >( HASH_BT( "m_lj" ) ), 15 );

				g_imgui.checkbox( _( "mini jump" ), &g_config.find< bool >( HASH_BT( "m_mj" ) ) );
				g_imgui.keybind( _( "mini jump key" ), &g_config.find< int >( HASH_BT( "m_mj_key" ) ), 15 );
				g_imgui.checkbox( _( "fast duck" ), &g_config.find< bool >( HASH_BT( "m_fast_duck" ) ) );
				g_imgui.checkbox( _( "auto align" ), &g_config.find< bool >( HASH_BT( "m_auto_align" ) ) );

				g_imgui.end_child( );
			}

			g_imgui.same_line( );

			g_imgui.begin_child( _( "indicators-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x, g_imgui.get_content_region_avail( ).y / 2 ) );
			{
				g_imgui.separator(
					window_draw_list,
					ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "indicators" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
					ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ), _( "indicators" ) );

				/* todo - coffin - check if custom clr is on, then render 1 clr picker, else do 3 negative, positive, neutral */
				g_imgui.checkbox( _( "velocity##indicator" ), &g_config.find< bool >( HASH_BT( "m_velocity_indicator" ) ) );
				if ( g_config.find< bool >( HASH_BT( "m_velocity_indicator" ) ) ) {
					g_imgui.checkbox( _( "show pre##velocity" ), &g_config.find< bool >( HASH_BT( "m_velocity_indicator_show_pre" ) ), 15 );
					g_imgui.checkbox( _( "fade##velocity" ), &g_config.find< bool >( HASH_BT( "m_velocity_indicator_fade" ) ), 15 );
				}

				g_imgui.checkbox( _( "stamina##indicator" ), &g_config.find< bool >( HASH_BT( "m_stamina_indicator" ) ) );
				if ( g_config.find< bool >( HASH_BT( "m_stamina_indicator" ) ) ) { }

				g_imgui.checkbox( _( "keybinds" ), &g_config.find< bool >( HASH_BT( "m_keybind_indicator" ) ) );
				// g_imgui.combo( _( "displayed keybinds" ), &g_config.find< int >( HASH_BT( "m_displayed_keybinds" ) ), _( "jb\0ej\0ej\0mj" ), 2 );

				static const char* indicated[]{ _( "jumpbug" ),   _( "minijump" ),   _( "longjump" ), _( "edgejump" ),
					                            _( "pixelsurf" ), _( "autostrafe" ), _( "edgebug" ) };

				std::vector< bool > keys = { true, true, true, false, true, true, true };

				if ( ImGui::BeginCombo( _( "displayed keybinds" ), _( "..." ), ImGuiComboFlags_NoArrowButton ) ) {
					for ( std::size_t i = 0; i < IM_ARRAYSIZE( indicated ); i++ )
						ImGui::Selectable( indicated[ i ], keys[ i ], ImGuiSelectableFlags_DontClosePopups );

					ImGui::EndCombo( );
				}

				g_imgui.spacing( );
				g_imgui.end_child( );
			}

			g_imgui.set_cursor_pos( ImVec2( g_imgui.get_content_region_avail( ).x / 2, 258 ) );

			g_imgui.begin_child( _( "game-child-contents" ), ImVec2( g_imgui.get_content_region_avail( ).x, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator(
					window_draw_list, ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "game" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
					ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ), _( "game" ) );

				g_imgui.spacing( );

				g_imgui.end_child( );
			}
			break;
		}
		case 4: { /* skins tab */
			break;
		}
		case 5: { /* config tab */
			g_imgui.begin_child( _( "config-child-contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x / 2, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator( window_draw_list,
				                   ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "config" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
				                   ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ),
				                   _( "config" ) );

				g_imgui.push_item_width( g_imgui.get_content_region_avail( ).x );
				g_imgui.push_style_var( ImGuiStyleVar_FramePadding, ImVec2( ImGui::GetStyle( ).FramePadding.x, 2 ) );

				g_imgui.listbox(
					_( "##configuration-list" ), &g_menu.selected_cfg, []( int index ) { return g_files.info.file_names[ index ].c_str( ); },
					g_files.info.file_names.size( ), 5 );

				g_imgui.input_text_with_hint( _( "##configuration-file-name" ), _( "enter file name..." ), g_menu.cfg_name.data( ),
				                              sizeof( g_menu.cfg_name.data( ) ) );

				ImGui::PopStyleVar( );
				ImGui::PopItemWidth( );

				if ( ImGui::Button( _( "create" ), ImVec2( ImGui::GetContentRegionAvail( ).x, 15 ) ) ) {
					if ( !g_config.save( g_menu.cfg_name.data( ) ) ) {
						g_log.print( console::format( _( "failed to save config {}" ), g_menu.cfg_name ), _( "[config error]" ), 12.f );
					}

					g_menu.cfg_name.clear( );
					g_files.refresh( );
				}

				if ( ImGui::Button( _( "save" ), ImVec2( ImGui::GetContentRegionAvail( ).x / 2, 15 ) ) )
					ImGui::OpenPopup( _( "confirmation##config.save" ) );

				g_imgui.same_line( );

				if ( ImGui::Button( _( "load" ), ImVec2( ImGui::GetContentRegionAvail( ).x, 15 ) ) ) {
					if ( !g_config.load( g_menu.cfg_name.data( ) ) ) {
						g_log.print( console::format( _( "failed to load config {}" ), g_menu.cfg_name ), _( "[config error]" ), 12.f );
					}
				}

				if ( ImGui::Button( _( "remove" ), ImVec2( ImGui::GetContentRegionAvail( ).x, 15 ) ) )
					ImGui::OpenPopup( _( "confirmation##config.remove" ) );

				if ( ImGui::Button( _( "refresh" ), ImVec2( ImGui::GetContentRegionAvail( ).x, 15 ) ) ) {
					g_files.refresh( );
				}

				g_imgui.color_picker( _( "menu accent" ), g_config.find< color >( HASH_BT( "m_menu_color" ) ), false, 17, true );

				g_imgui.end_child( );
			}

			g_imgui.same_line( );

			g_imgui.begin_child( _( "scripting-child_contents" ),
			                     ImVec2( g_imgui.get_content_region_avail( ).x, g_imgui.get_content_region_avail( ).y ) );
			{
				g_imgui.separator( window_draw_list,
				                   ImVec2( ImGui::GetWindowPos( ).x + g_imgui.calc_text_size( _( "config" ) ).x + 10, ImGui::GetWindowPos( ).y + 7 ),
				                   ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetContentRegionAvail( ).x, ImGui::GetWindowPos( ).y + 7 + 1 ),
				                   _( "scripts" ) );

				g_imgui.end_child( );
			}

			break;
		}
		}
	}

	g_imgui.end( );
}
