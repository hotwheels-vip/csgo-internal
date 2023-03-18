#include "../../globals/includes/includes.h"
#include "../../game/sdk/includes/includes.h"
#include "../hooks.h"

#include <intrin.h>

bool n_detoured_functions::is_paused( void* ecx, void* edx )
{
	static auto original = g_hooks.m_is_paused.get_original< bool( __thiscall* )( void* ) >( );

	if ( !g_ctx.m_local || !g_ctx.m_local->is_alive( ) )
		return original( ecx );

	static const auto is_paused_extrapolate =
		reinterpret_cast< unsigned int >( g_modules[ CLIENT_DLL ].find_pattern( "0F B6 0D ? ? ? ? 84 C0 0F 44" ) );

	if ( const auto return_address = reinterpret_cast< unsigned int >( _ReturnAddress( ) ); return_address == is_paused_extrapolate )
		return true;

	return original( ecx );
}