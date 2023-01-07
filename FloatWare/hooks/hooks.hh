#pragma once

#include "../utilities/classes/detour_hook.hh"

namespace hooks
{
	struct impl {
	public:
		bool on_attach( );
		void on_release( );

		c_detour_hook vsnprintf_detour = { };
	};

	int __cdecl vsnprintf( char* dest, int text_length, const char* fmt, ... );
} // namespace hooks

inline hooks::impl g_hooks;