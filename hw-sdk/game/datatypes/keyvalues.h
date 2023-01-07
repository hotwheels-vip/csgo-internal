#pragma once
#include "../../dependencies/lazy_importer/lazy_importer.h"
#include "../../globals/macros/macros.h"
#include "../../utils/modules/modules.h"
#include "../../utils/vfunc/vfunc.h"
#include "../sdk/enums/key_type.h"
#include <libloaderapi.h>

namespace sdk
{
	class c_key_values
	{
	public:
		static c_key_values* from_string( const char* name, const char* value )
		{
			const auto pattern = g_client_dll.pattern_scan( _( "E8 ? ? ? ? 83 C4 04 89 45 D8" ) ).add( 1 ).as< unsigned int >( );

			c_key_values* key_values{ };
			// blame it on classy
			__asm
			{
				push 0
				mov edx, value
				mov ecx, name
				call pattern
				add esp, 4
				mov key_values, eax
			}

			return key_values;
		}

		c_key_values* find_key( const char* key_name, bool create )
		{
			c_key_values*( __thiscall * find )( c_key_values * key, const char* name, bool create ) =
				g_client_dll.pattern_scan( _( "E8 ? ? ? ? F7 45" ) ).add( 1 ).as< decltype( find ) >( );

			return find( this, key_name, create );
		}

		void set_string( const char* key_name, const char* value )
		{
			void( __thiscall * set )( c_key_values*, const char*, const char* ) =
				g_client_dll.pattern_scan( _( "E8 ? ? ? ? 89 77 38" ) ).add( 1 ).as< decltype( set ) >( );

			if ( const auto key = find_key( key_name, false ) )
				set( this, key_name, value );
		}
	};
} // namespace sdk
