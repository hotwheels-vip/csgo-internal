#pragma once

#include "../dependencies/lazy_importer.h"
#include "../dependencies/xorstr.h"
#include <spdlog/spdlog.h>

namespace console
{
	struct impl {
	public:
		void on_attach( const char* title );
		void on_release( );

		template< typename... Args_t >
		void print( const char* text, const Args_t&... args )
		{
#ifdef _DEBUG
			if constexpr ( sizeof...( args ) > 0 )
				spdlog::info( xs( "hotwheels | {:s}" ), std::vformat( text, std::make_format_args( args... ) ) );
			else
				spdlog::info( xs( "hotwheels | {:s}" ), text );
#endif
		}
	};
} // namespace console

inline console::impl g_console;