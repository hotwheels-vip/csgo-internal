#include "imgui_helper.h"
#include "../../../dependencies/imgui/dx9/imgui_impl_dx9.h"
#include "../../../dependencies/imgui/imgui.h"
#include "../../../dependencies/imgui/imgui_freetype.h"
#include "../../../dependencies/imgui/imgui_impl_win32.h"
#include "../../../dependencies/imgui/imgui_internal.h"
#include "../../../dependencies/mocking_bird/mocking_bird.h"
#include <map>

bool imgui::impl::new_frame( )
{
	MOCKING_TRY;

	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	MOCKING_CATCH( return false );

	return true;
}

bool imgui::impl::end_frame( )
{
	MOCKING_TRY;

	ImGui::EndFrame( );
	ImGui::Render( );
	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

	MOCKING_CATCH( return false );

	return true;
}

ImVec2 imgui::impl::get_cursor_pos( )
{
	return ImGui::GetCursorPos( );
}

float imgui::impl::get_cursor_pos_x( )
{
	return ImGui::GetCursorPosX( );
}

float imgui::impl::get_cursor_pos_y( )
{
	return ImGui::GetCursorPosY( );
}

void imgui::impl::set_cursor_pos( const ImVec2& local_pos )
{
	ImGui::SetCursorPos( local_pos );
}

void imgui::impl::set_cursor_pos_x( float local_x )
{
	ImGui::SetCursorPosX( local_x );
}

void imgui::impl::set_cursor_pos_y( float local_y )
{
	ImGui::SetCursorPosY( local_y );
}

void imgui::impl::render_tab( const char* label, int tab_number )
{
	ImGui::RenderTab( label, tab_number );
}

void imgui::impl::end_child( bool do_shadow )
{
	ImGui::EndChild( do_shadow );
}

bool imgui::impl::begin_child( const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags )
{
	return ImGui::BeginChild( str_id, size_arg, border, extra_flags );
}

void imgui::impl::same_line( float offset_from_start_x, float spacing )
{
	ImGui::SameLine( offset_from_start_x, spacing );
}

bool imgui::impl::begin( const char* name, bool* p_open, ImGuiWindowFlags flags )
{
	return ImGui::Begin( name, p_open, flags );
}

void imgui::impl::set_next_window_size( const ImVec2& size, ImGuiCond cond )
{
	ImGui::SetNextWindowSize( size, cond );
}

void imgui::impl::destroy( )
{
	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );
}

void imgui::impl::text( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	ImGui::TextV( fmt, args );
	va_end( args );
}

bool imgui::impl::checkbox( const char* label, bool* v, int x_pos )
{
	return ImGui::Checkbox( label, v, x_pos );
}

void imgui::impl::combo( const char* label, int* current_item, const char* items, int same_line )
{
	auto id = ImGui::GetCurrentWindow( )->GetID( label );

	static std::map< ImGuiID, float > opening_alpha;

	auto opened_frame = opening_alpha.find( id );

	if ( opened_frame == opening_alpha.end( ) ) {
		opening_alpha.insert( { id, 0.f } );
		opened_frame = opening_alpha.find( id );
	}

	opened_frame->second =
		ImClamp( opened_frame->second + ( 2.35f * ImGui::GetIO( ).DeltaTime * ( ImGui::IsPopupOpen( label ) ? 2.f : -2.f ) ), 0.0f, 1.f );
	opened_frame->second *= ImGui::GetStyle( ).Alpha;

	ImGui::PushStyleColor( ImGuiCol_PopupBg, ImVec4( 20 / 255.f, 20 / 255.f, 20 / 255.f, opened_frame->second ) );
	ImGui::PushStyleColor( ImGuiCol_WindowBg, ImVec4( 20 / 255.f, 20 / 255.f, 20 / 255.f, opened_frame->second ) );
	ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( ImGui::GetStyle( ).FramePadding.x, 1 ) );
	ImGui::SetCursorPosX( same_line );
	ImGui::PushItemWidth( -1 );
	ImGui::Combo( label, current_item, items );
	ImGui::PopItemWidth( );
	ImGui::PopStyleVar( );
	ImGui::PopStyleColor( );
	ImGui::PopStyleColor( );
}

void imgui::impl::listbox( const char* label, int* current_item, std::function< const char*( int ) > function, int item_count, int height_in_items )
{
	auto render = [ & ]( ) {
		return ImGui::ListBox(
			label, current_item,
			[]( void* data, int index, const char** out ) {
				*out = ( *static_cast< std::function< const char*( int ) >* >( data ) )( index );
				return true;
			},
			&function, item_count, height_in_items );
	};

	render( );
}

bool imgui::impl::keybind( const char* id, int* current_key, int sameline )
{
	return ImGui::Keybind( id, current_key, sameline );
}

void imgui::impl::separator( ImDrawList* draw_list, const ImVec2& min, const ImVec2& max, const char* text )
{
	if ( text ) {
		ImGui::SetCursorPosX( 6 );
		ImGui::Text( text );
	}

	ImVec2 text_size = imgui::impl::calc_text_size( text );

	draw_list->AddRectFilledMultiColor( min, max, g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( 1.f ),
	                                    g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( 0.f ),
	                                    g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( 0.f ),
	                                    g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( 1.f ) );
}

ImVec2 imgui::impl::calc_text_size( const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width )
{
	return ImGui::CalcTextSize( text, text_end, hide_text_after_double_hash, wrap_width );
}

void imgui::impl::push_item_width( float item_width )
{
	ImGui::PushItemWidth( item_width );
}

void imgui::impl::push_style_var( ImGuiStyleVar idx, const ImVec2& val )
{
	ImGui::PushStyleVar( idx, val );
}

bool imgui::impl::input_text_with_hint( const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags,
                                        ImGuiInputTextCallback callback, void* user_data )
{
	return ImGui::InputTextWithHint( label, hint, buf, buf_size, flags, callback, user_data );
}

ImVec2 imgui::impl::get_content_region_avail( )
{
	return ImGui::GetContentRegionAvail( );
}

void imgui::impl::end( )
{
	ImGui::End( );
}

void imgui::impl::spacing( int times )
{
	for ( int i = 0; i <= times; i++ )
		ImGui::Spacing( );
}

void imgui::impl::start_decorations( ImDrawList* draw_list )
{
	draw_list->AddRectFilledMultiColor(
		ImVec2( ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + 25 ),
		ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetWindowSize( ).x / 2, ImGui::GetWindowPos( ).y + 25 + 1 ),
		g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( 0.f ), g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( ),
		g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( ), g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( 0.f ) );

	draw_list->AddRectFilledMultiColor(
		ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetWindowSize( ).x / 2, ImGui::GetWindowPos( ).y + 25 ),
		ImVec2( ( ImGui::GetWindowPos( ).x + ImGui::GetWindowSize( ).x / 2 ) + ImGui::GetWindowSize( ).x / 2, ImGui::GetWindowPos( ).y + 25 + 1 ),
		g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( ), g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( 0.f ),
		g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( 0.f ), g_config.find< color >( HASH_BT( "m_menu_color" ) ).get_u32( ) );

	ImGui::SetCursorPosX( ImGui::GetContentRegionAvail( ).x / 2 - ImGui::CalcTextSize( _( "hotwheels.vip" ) ).x / 2 );
	ImGui::Text( _( "hotwheels" ) );
	ImGui::SameLine( 0, 0 );
	ImGui::TextColored( g_config.find< color >( HASH_BT( "m_menu_color" ) ).to_imvec4( ), _( ".vip" ) );
}

bool imgui::impl::color_picker( const char* label, color& col, bool alpha, int same_line, bool show_text )
{
	if ( show_text )
		ImGui::Text( label );

	auto slider_flag = alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha;
	ImGui::SameLine( ImGui::GetWindowWidth( ) - same_line );

	float f[ 4 ] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f };

	ImGui::ColorEdit4( std::string( _( "##" ) ).append( label ).c_str( ), f,
	                   slider_flag | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip );

	col = color( f[ 0 ] * 255.f, f[ 1 ] * 255.f, f[ 2 ] * 255.f, f[ 3 ] * 255.f );

	return true;
}

void imgui::impl::menu_init( IDirect3DDevice9* device )
{
	ImGui::CreateContext( );

	ImGui_ImplWin32_Init( g_menu.window );
	ImGui_ImplDX9_Init( device );

	ImGuiStyle& style = ImGui::GetStyle( );
	ImGuiIO& io       = ImGui::GetIO( );

	/* colors */
	{
		style.Colors[ ImGuiCol_Border ] = ImVec4( 30 / 255.f, 30 / 255.f, 30 / 255.f, 0.5f );

		style.Colors[ ImGuiCol_ScrollbarBg ]          = ImVec4( 1.f, 1.f, 1.f, 1.f );
		style.Colors[ ImGuiCol_ScrollbarGrab ]        = ImVec4( 1.f, 1.f, 1.f, 1.f );
		style.Colors[ ImGuiCol_ScrollbarGrabActive ]  = ImVec4( 1.f, 1.f, 1.f, 1.f );
		style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 1.f, 1.f, 1.f, 1.f );

		style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 10 / 255.f, 10 / 255.f, 10 / 255.f, 1.00f );
		style.Colors[ ImGuiCol_PopupBg ]  = ImVec4( 20 / 255.f, 20 / 255.f, 20 / 255.f, 1.00f );
		style.Colors[ ImGuiCol_ChildBg ]  = ImVec4( 15 / 255.f, 15 / 255.f, 15 / 255.f, 1.0f );

		style.Colors[ ImGuiCol_FrameBg ] = g_config.find< color >( HASH_BT( "m_menu_color" ) ).to_imvec4( );
	}

	/* style */
	{
		style.WindowRounding    = 3.f;
		style.ChildRounding     = 2.f;
		style.FrameRounding     = 2.f;
		style.GrabRounding      = 2.f;
		style.PopupRounding     = 2.f;
		style.ScrollbarRounding = 4.f;
		style.TabRounding       = 3.f;

		style.WindowBorderSize = 2.f;
		style.FrameBorderSize  = 0.f;
		style.ChildBorderSize  = 1.;

		style.ScrollbarSize = 1.f;

		style.FramePadding     = ImVec2( 4, -1 );
		style.WindowPadding    = ImVec2( 8, 8 );
		style.ItemSpacing      = ImVec2( 8, 4 );
		style.ItemInnerSpacing = ImVec2( 4, 4 );
		style.WindowMinSize    = ImVec2( 32, 32 );
		style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
		style.ButtonTextAlign  = ImVec2( 0.5f, 0.4f );

		style.Alpha = 1.f;
	}

	/* add fonts here */
	ImFontConfig im_tahoma_font_config    = { };
	im_tahoma_font_config.RasterizerFlags = ImGuiFreeType::Monochrome | ImGuiFreeType::MonoHinting;
	g_menu.menu_font                      = io.Fonts->AddFontFromFileTTF( _( "C:\\Windows\\Fonts\\Tahoma.ttf" ), 12.f, &im_tahoma_font_config );

	g_menu.menu_initialised = ImGuiFreeType::BuildFontAtlas( io.Fonts, 0x0 );
}
