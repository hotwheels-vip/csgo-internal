#pragma once
#include <format>
#include <iostream>
#include <windows.h>

#include <spdlog/spdlog.h>

#include "../../hashing/imports/lazy_importer.h"
#include "../../hashing/strings/xorstr.h"

struct console_t {
	void on_attach( const char* window_title )
	{
#ifdef _DEBUG
		LI_FN( AllocConsole )( );

		freopen_s( reinterpret_cast< _iobuf** >( stdout ), xs( "conout$" ), xs( "w" ), static_cast< _iobuf* >( stdout ) );

		if ( window_title )
			LI_FN( SetConsoleTitleA )( window_title );
#endif
	}

	void on_release( )
	{
#ifdef _DEBUG
		fclose( static_cast< _iobuf* >( stdout ) );

		LI_FN( FreeConsole )( );
#endif
	}

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

inline console_t console = { };