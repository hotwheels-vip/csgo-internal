#pragma once
#include <cstdarg>
#include <stdio.h>

namespace sdk
{
	class c_hud_chat
	{
	public:
		void chat_print( const char* fmt, ... )
		{
			if ( !this )
				return;
			static char buf[ 1024 ]{ };

			va_list va;
			va_start( va, fmt );
			vsnprintf_s( buf, 1024, fmt, va );
			va_end( va );

			using fn = void( __cdecl* )( void*, int, int, const char*, ... );
			return ( *( fn** )this )[ 27 ]( this, 0, 0, buf );
		}
	};
} // namespace sdk
