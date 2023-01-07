#pragma once

#include "../includes/includes.h"

// cheat initialization method

namespace hotwheels
{
	inline HMODULE handle{ };
	inline HWND window{ };

	DWORD WINAPI init( void* module_handle );
	DWORD WINAPI unload( DWORD exit_code = EXIT_FAILURE );

}; // namespace hotwheels
