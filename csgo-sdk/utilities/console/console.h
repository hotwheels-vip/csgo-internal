#pragma once

namespace n_console
{
	struct impl_t {
		void on_attach( const char* console_title );
		void on_release( );

		void print( const char* text );
	};
} // namespace n_console

inline n_console::impl_t g_console{ };