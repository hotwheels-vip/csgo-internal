#pragma once

// deps
#include "log_level/log_level.h"

#include <Windows.h>
#include <consoleapi.h>
#include <consoleapi2.h>
#include <consoleapi3.h>
#include <stdio.h>
#include <utility>
#include <windef.h>
#include <xstring>

namespace console
{
	bool init( );

	void unload( );
	inline HANDLE console_handle;

	template< console::log_level lev, typename... VA >
	void print( std::string_view print, VA&&... args );

	template< typename... VA >
	inline std::string format( std::string_view fmt, VA&&... args );

	inline void set_console_color( const console::log_level level );
} // namespace console

#include "console.inl"
