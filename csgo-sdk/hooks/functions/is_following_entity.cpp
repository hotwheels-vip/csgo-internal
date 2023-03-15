#include "../../game/sdk/includes/includes.h"
#include "../../globals/config/variables.h"
#include "../hooks.h"

#include <intrin.h>

bool __fastcall n_detoured_functions::is_following_entity( void* ecx, void* edx )
{
	static auto original = g_hooks.m_is_following_entity.get_original< bool( __fastcall* )( void* ) >( );

#ifdef _DEBUG
	if ( GET_VARIABLE( g_variables.m_disable_interp, bool ) && _ReturnAddress( ) == reinterpret_cast< void* >( g_modules[ CLIENT_DLL ].find_pattern(
																						"84 C0 0F 85 ? ? ? ? 38 05 ? ? ? ? 0F 84 ? ? ? ? 53" ) ) )
		return true;
#endif

	return original( ecx );
}