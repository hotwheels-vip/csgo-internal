#pragma once
#include "../menu.h"

namespace imgui
{
	struct impl {
		bool new_frame( );
		bool end_frame( );

		// main
		void start_decorations( ImDrawList* draw_list );

		void destroy( );

		// utils
		void spacing( int times = 0 );
		void set_next_window_size( const ImVec2& size, ImGuiCond cond = 0 );
		void same_line( float offset_from_start_x = 0.0f, float spacing = -1.0f );
		ImVec2 get_content_region_avail( );
		ImVec2 calc_text_size( const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f );

		// cursors
		ImVec2 get_cursor_pos( );

		float get_cursor_pos_x( );
		float get_cursor_pos_y( );

		void set_cursor_pos( const ImVec2& local_pos );

		void set_cursor_pos_x( float local_x );
		void set_cursor_pos_y( float local_y );

		void push_item_width( float item_width );
		void push_style_var( ImGuiStyleVar idx, const ImVec2& val );
		// elements
		bool begin( const char* name, bool* p_open, ImGuiWindowFlags flags = 0 );
		void end( );

		bool begin_child( const char* str_id, const ImVec2& size_arg, bool border = false, ImGuiWindowFlags extra_flags = 0 );
		void end_child( bool do_shadow = false );

		bool input_text_with_hint( const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0,
		                           ImGuiInputTextCallback callback = NULL, void* user_data = NULL );
		void render_tab( const char* label, int tab_number );
		void text( const char* fmt, ... );
		bool checkbox( const char* label, bool* v, int x_pos = 5 );
		bool color_picker( const char* label, color& col, bool alpha, int same_line, bool show_text );
		void combo( const char* label, int* current_item, const char* items, int same_line );
		void menu_init( IDirect3DDevice9* device );
		bool keybind( const char* id, int* current_key, int sameline = 5 );
		void separator( ImDrawList* draw_list, const ImVec2& min, const ImVec2& max, const char* text = nullptr );
		void listbox( const char* label, int* current_item, std::function< const char*( int ) > function, int item_count, int height_in_items );
	};
} // namespace imgui

inline imgui::impl g_imgui;
