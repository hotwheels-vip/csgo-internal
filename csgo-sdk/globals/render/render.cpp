#include "render.h"
#include "../../dependencies/imgui/helpers/fonts.h"

#include "../../game/sdk/includes/includes.h"
#include "../includes/includes.h"

void n_render::impl_t::on_end_scene( const std::function< void( ) >& function, IDirect3DDevice9* device )
{
	if ( !this->m_initialised ) {
		ImGui::CreateContext( );
		ImGui_ImplWin32_Init( g_input.m_window );
		ImGui_ImplDX9_Init( device );

		auto& style = ImGui::GetStyle( );

		/* setup styles */
		[ & ]( ) {
			style.WindowRounding    = 5.f;
			style.ChildRounding     = 0.f;
			style.FrameRounding     = 2.f;
			style.GrabRounding      = 3.f;
			style.PopupRounding     = 3.f;
			style.ScrollbarRounding = 0.f;

			style.FrameBorderSize  = 0.f;
			style.WindowBorderSize = 0.f;
			style.PopupBorderSize  = 0.f;
			style.ScrollbarSize    = 8.f;
			style.GrabMinSize      = 0.f;

			style.WindowPadding   = ImVec2( 8, 8 );
			style.FramePadding    = ImVec2( 0, 0 );
			style.ButtonTextAlign = ImVec2( 0.5f, 0.5f );
			style.ItemSpacing     = ImVec2( 8, 8 );

			style.AntiAliasedFill        = true;
			style.AntiAliasedLines       = true;
			style.AntiAliasedLinesUseTex = true;
		}( );

		/* setup colours */
		[ & ]( ) {
			style.Colors[ ImGuiCol_::ImGuiCol_WindowBg ]  = ImVec4( 10 / 255.f, 10 / 255.f, 10 / 255.f, 1.f );
			style.Colors[ ImGuiCol_::ImGuiCol_ChildBg ]   = ImVec4( 15 / 255.f, 15 / 255.f, 15 / 255.f, 1.f );
			style.Colors[ ImGuiCol_::ImGuiCol_FrameBg ]   = ImVec4( 25 / 255.f, 25 / 255.f, 25 / 255.f, 1.f );
			style.Colors[ ImGuiCol_::ImGuiCol_PopupBg ]   = ImVec4( 20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f );
			style.Colors[ ImGuiCol_::ImGuiCol_CheckMark ] = ImVec4( 0 / 255.f, 0 / 255.f, 0 / 255.f, 1.f );
			style.Colors[ ImGuiCol_::ImGuiCol_Button ]    = ImVec4( 20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f );

			style.Colors[ ImGuiCol_::ImGuiCol_Border ]       = ImVec4( 0 / 255.f, 0 / 255.f, 0 / 255.f, 0.f );
			style.Colors[ ImGuiCol_::ImGuiCol_BorderShadow ] = ImVec4( 0 / 255.f, 0 / 255.f, 0 / 255.f, 0.f );
		}( );

		[ & ]( ) {
			ImGuiIO& io = ImGui::GetIO( );

			ImFontConfig verdana_font_config = { };
			verdana_font_config.FontBuilderFlags =
				ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting;

			m_fonts[ e_font_names::font_name_verdana_11 ] =
				io.Fonts->AddFontFromMemoryCompressedTTF( verdana_compressed_data, verdana_compressed_size, 11.f, &verdana_font_config );
			m_fonts[ e_font_names::font_name_verdana_bd_11 ] =
				io.Fonts->AddFontFromMemoryCompressedTTF( verdana_bold_compressed_data, verdana_bold_compressed_size, 11.f, &verdana_font_config );

			m_fonts[ e_font_names::font_name_verdana_12 ] =
				io.Fonts->AddFontFromMemoryCompressedTTF( verdana_compressed_data, verdana_compressed_size, 12.f, &verdana_font_config );
			m_fonts[ e_font_names::font_name_verdana_bd_12 ] =
				io.Fonts->AddFontFromMemoryCompressedTTF( verdana_bold_compressed_data, verdana_bold_compressed_size, 12.f, &verdana_font_config );

			m_fonts[ e_font_names::font_name_smallest_pixel_10 ] =
				io.Fonts->AddFontFromMemoryCompressedTTF( smallest_pixel_compressed_data, smallest_pixel_compressed_size, 10.f );

			ImFontConfig icon_font_config     = { };
			icon_font_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_LightHinting |
			                                    ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_Monochrome |
			                                    ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting;
			constexpr ImWchar icon_ranges[] = { 0xE000, 0xF8FF, 0 };

			m_fonts[ e_font_names::font_name_icon_12 ] = io.Fonts->AddFontFromMemoryCompressedTTF(
				weapon_icons_compressed_data, weapon_icons_compressed_size, 12.f, &icon_font_config, icon_ranges );

			m_fonts[ e_font_names::font_name_indicator_29 ] =
				io.Fonts->AddFontFromMemoryCompressedTTF( verdana_bold_compressed_data, verdana_bold_compressed_size, 29.f );

			ImFontConfig tahoma_font_config = { };
			tahoma_font_config.FontBuilderFlags =
				ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting;

			m_fonts[ e_font_names::font_name_tahoma_12 ] =
				io.Fonts->AddFontFromMemoryCompressedTTF( tahoma_compressed_data, tahoma_compressed_size, 12.f, &tahoma_font_config );
			m_fonts[ e_font_names::font_name_tahoma_bd_12 ] =
				io.Fonts->AddFontFromMemoryCompressedTTF( tahoma_bold_compressed_data, tahoma_bold_compressed_size, 12.f, &tahoma_font_config );

			ImGuiFreeType::BuildFontAtlas( io.Fonts, 0x0 );
		}( );

		this->m_initialised = true;
	}

	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	const auto display_size = ImGui::GetIO( ).DisplaySize;

	g_ctx.m_width  = display_size.x;
	g_ctx.m_height = display_size.y;

	this->draw_cached_data( );

	function( );

	ImGui::EndFrame( );
	ImGui::Render( );

	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
}

void n_render::impl_t::on_release( )
{
	this->clear_draw_data( );

	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );
}

void n_render::impl_t::draw_cached_data( )
{
	const auto draw_list = ImGui::GetBackgroundDrawList( );

	std::unique_lock< std::shared_mutex > lock( this->m_mutex );

	if ( this->m_thread_safe_draw_data.empty( ) )
		return;

	for ( const draw_object_t& data : this->m_thread_safe_draw_data ) {
		if ( !data.m_obj.has_value( ) )
			continue;

		switch ( data.m_type ) {
		case e_draw_type::draw_type_text: {
			const auto& obj = std::any_cast< text_draw_object_t >( data.m_obj );
			this->text( draw_list, obj.m_font, obj.m_position, obj.m_text, obj.m_color, obj.m_outline_color, obj.m_draw_flags );
			break;
		}
		case e_draw_type::draw_type_line: {
			const auto& obj = std::any_cast< line_draw_object_t >( data.m_obj );
			draw_list->AddLine( ImVec2( obj.m_start.m_x, obj.m_start.m_y ), ImVec2( obj.m_end.m_x, obj.m_end.m_y ), obj.m_color, obj.m_thickness );
			break;
		}
		case e_draw_type::draw_type_rect: {
			const auto& obj = std::any_cast< rect_draw_object_t >( data.m_obj );
			this->rect( draw_list, obj.m_min, obj.m_max, obj.m_color, obj.m_outline_color, obj.m_filled, obj.m_rounding, obj.m_corner_rounding_flags,
			            obj.m_thickness, obj.m_outline_flags );
			break;
		}
		default:
			break;
		}
	}
}

bool n_render::impl_t::world_to_screen( const c_vector& origin, c_vector_2d& screen )
{
	const auto& world_to_screen_matrix = g_interfaces.m_engine_client->get_world_to_screen_matrix( );

	const float width = world_to_screen_matrix[ 3 ][ 0 ] * origin.m_x + world_to_screen_matrix[ 3 ][ 1 ] * origin.m_y +
	                    world_to_screen_matrix[ 3 ][ 2 ] * origin.m_z + world_to_screen_matrix[ 3 ][ 3 ];

	if ( width < 0.001f )
		return false;

	const float inverse = 1.0f / width;
	screen.m_x          = ( world_to_screen_matrix[ 0 ][ 0 ] * origin.m_x + world_to_screen_matrix[ 0 ][ 1 ] * origin.m_y +
                   world_to_screen_matrix[ 0 ][ 2 ] * origin.m_z + world_to_screen_matrix[ 0 ][ 3 ] ) *
	             inverse;
	screen.m_y = ( world_to_screen_matrix[ 1 ][ 0 ] * origin.m_x + world_to_screen_matrix[ 1 ][ 1 ] * origin.m_y +
	               world_to_screen_matrix[ 1 ][ 2 ] * origin.m_z + world_to_screen_matrix[ 1 ][ 3 ] ) *
	             inverse;

	screen.m_x = ( g_ctx.m_width * 0.5f ) + ( screen.m_x * g_ctx.m_width ) * 0.5f;
	screen.m_y = ( g_ctx.m_height * 0.5f ) - ( screen.m_y * g_ctx.m_height ) * 0.5f;
	return true;
}

void n_render::impl_t::text( ImDrawList* draw_list, ImFont* font, const c_vector_2d& position, const std::string& text, const unsigned int& color,
                             const unsigned int& outline_color, e_text_flags draw_flags )
{
	draw_list->PushTextureID( font->ContainerAtlas->TexID );

	if ( draw_flags & e_text_flags::text_flag_dropshadow )
		draw_list->AddText( font, font->FontSize, ImVec2( position.m_x + 1.f, position.m_y + 1.f ), outline_color, text.c_str( ) );
	else if ( draw_flags & e_text_flags::text_flag_outline ) {
		draw_list->AddText( font, font->FontSize, ImVec2( position.m_x + 1.f, position.m_y - 1.f ), outline_color, text.c_str( ) );
		draw_list->AddText( font, font->FontSize, ImVec2( position.m_x - 1.f, position.m_y + 1.f ), outline_color, text.c_str( ) );
	}

	draw_list->AddText( font, font->FontSize, ImVec2( position.m_x, position.m_y ), color, text.c_str( ) );

	draw_list->PopTextureID( );
}

void n_render::impl_t::rect( ImDrawList* draw_list, const c_vector_2d& min, const c_vector_2d& max, const unsigned int& color,
                             const unsigned int& outline_color, bool filled, float rounding, int corner_rounding_flags, float thickness,
                             unsigned int outline_flags )
{
	if ( filled )
		draw_list->AddRectFilled( ImVec2( min.m_x, min.m_y ), ImVec2( max.m_x, max.m_y ), color, rounding, corner_rounding_flags );
	else
		draw_list->AddRect( ImVec2( min.m_x, min.m_y ), ImVec2( max.m_x, max.m_y ), color, rounding, corner_rounding_flags, thickness );

	if ( outline_flags & e_rect_flags::rect_flag_inner_outline )
		draw_list->AddRect( ImVec2( min.m_x + 1.f, min.m_y + 1.f ), ImVec2( max.m_x - 1.f, max.m_y - 1.f ), outline_color );

	if ( outline_flags & e_rect_flags::rect_flag_outer_outline )
		draw_list->AddRect( ImVec2( min.m_x - 1.f, min.m_y - 1.f ), ImVec2( max.m_x + 1.f, max.m_y + 1.f ), outline_color );
}

void n_render::impl_t::corner_rect( float x1, float y1, float x2, float y2, const unsigned int& color, float thickness )
{
	int w = x2 - x1;
	int h = y2 - y1;

	int iw = w / 3;
	int ih = h / 5;

	this->m_draw_data.emplace_back( e_draw_type::draw_type_line,
	                                std::make_any< line_draw_object_t >( c_vector_2d( x1, y1 ), c_vector_2d( x1 + iw, y1 ), color, thickness ) );

	this->m_draw_data.emplace_back( e_draw_type::draw_type_line, std::make_any< line_draw_object_t >( c_vector_2d( x1 + w - iw, y1 ),
	                                                                                                  c_vector_2d( x1 + w, y1 ), color, thickness ) );

	this->m_draw_data.emplace_back( e_draw_type::draw_type_line,
	                                std::make_any< line_draw_object_t >( c_vector_2d( x1, y1 ), c_vector_2d( x1, y1 + ih ), color, thickness ) );

	this->m_draw_data.emplace_back(
		e_draw_type::draw_type_line,
		std::make_any< line_draw_object_t >( c_vector_2d( x1 + w - 1, y1 ), c_vector_2d( x1 + w - 1, y1 + ih ), color, thickness ) );

	this->m_draw_data.emplace_back( e_draw_type::draw_type_line, std::make_any< line_draw_object_t >(
																	 c_vector_2d( x1, y1 + h ), c_vector_2d( x1 + iw, y1 + h ), color, thickness ) );

	this->m_draw_data.emplace_back(
		e_draw_type::draw_type_line,
		std::make_any< line_draw_object_t >( c_vector_2d( x1 + w - iw, y1 + h ), c_vector_2d( x1 + w, y1 + h ), color, thickness ) );

	this->m_draw_data.emplace_back( e_draw_type::draw_type_line, std::make_any< line_draw_object_t >( c_vector_2d( x1, y1 + h - ih ),
	                                                                                                  c_vector_2d( x1, y1 + h ), color, thickness ) );

	this->m_draw_data.emplace_back(
		e_draw_type::draw_type_line,
		std::make_any< line_draw_object_t >( c_vector_2d( x1 + w - 1, y1 + h - ih ), c_vector_2d( x1 + w - 1, y1 + h ), color, thickness ) );
}
