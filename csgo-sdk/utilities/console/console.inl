#pragma once

/* fast, lightweight logging lib */
#include <spdlog/spdlog.h>

template< n_console::log_level lev >
void n_console::impl_t::print( const char* text )
{
	switch ( lev ) {
	case n_console::log_level::NORMAL:
		spdlog::info( text );
		break;
	case n_console::log_level::FATAL:
		spdlog::critical( text );
		break;
	case n_console::log_level::DEBUG:
		spdlog::debug( text );
		break;
	case n_console::log_level::WARNING:
		spdlog::warn( text );
		break;
	case n_console::log_level::SUCCESS:
		spdlog::info( text );
		break;
	default:
		spdlog::info( text );
	}
}
