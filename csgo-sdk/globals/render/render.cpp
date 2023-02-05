#include "render.h"
#include "../includes/includes.h"
#include "../../dependencies/imgui/helpers/fonts.h"

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

			ImGuiFreeType::BuildFontAtlas( io.Fonts, 0x0 );
		}( );

		this->m_initialised = true;
	}

	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	function();

	ImGui::EndFrame( );
	ImGui::Render( );

	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
}

void n_render::impl_t::on_release( )
{
	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );
}