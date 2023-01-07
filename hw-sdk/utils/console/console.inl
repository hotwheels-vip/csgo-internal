#pragma once
#include "../../globals/includes/includes.h"
#include "../../globals/macros/macros.h"

template< typename... VA >
inline std::string console::format( std::string_view fmt, VA&&... args )
{
	return fmt::vformat( fmt, fmt::make_format_args( std::forward< VA >( args )... ) );
};

void console::set_console_color( const console::log_level level )
{
	int color{ };
	switch ( level ) 
	{
	case console::log_level::NORMAL:
		color = console::log_colors::DARK_BLUE;
		break;
	case console::log_level::DEBUG:
		color = console::log_colors::CYAN;
		break;
	case console::log_level::SUCCESS:
		color = console::log_colors::GREEN;
		break;
	case console::log_level::WARNING:
		color = console::log_colors::YELLOW;
		break;
	case console::log_level::FATAL:
		color = console::log_colors::RED;
		break;
	}
	// dont ask
	int int_color = static_cast< int >( color );
	int_color %= 16;
	unsigned short att = ( ( unsigned )0 << 4 ) | ( unsigned( int_color ) );
	SetConsoleTextAttribute( console::console_handle, att );
};

inline std::ostream& operator<<( std::ostream& os, const console::log_level type )
	{
	switch ( type ) 
	{
	case console::log_level::NORMAL:
		return os << _("[.] ");
	case console::log_level::DEBUG:
		return os << _( "[>] " );
	case console::log_level::SUCCESS:
		return os << _( "[+] " );
	case console::log_level::WARNING:
		return os << _( "[*] " );
	case console::log_level::FATAL:
		return os << _( "[!] " );
	default:
		return os << _("");
	}
}

template< console::log_level lev, typename... VA >
void console::print( std::string_view print, VA&&... args )
{
#ifdef CONSOLE_ENABLED
	auto formatted = ::console::format( print, std::forward< VA >( args )... );

	console::set_console_color( lev );
	std::cout << lev;

	if ( lev == console::log_level::NORMAL )
		SetConsoleTextAttribute( console::console_handle, 8 );
	else
		SetConsoleTextAttribute( console::console_handle, 15 );

	std::cout << formatted << std::endl;

#endif
}