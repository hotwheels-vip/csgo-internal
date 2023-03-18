#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include <intrin.h>

bool n_detoured_functions::is_hltv( void* ecx, void* edx )
{
	static auto original = g_hooks.m_is_hltv.get_original< bool( __thiscall* )( void* ) >( );

	if ( !g_ctx.m_local || !g_ctx.m_local->is_alive( ) )
		return original( ecx );

	static const auto return_to_setup_velocity =
		*( reinterpret_cast< void** >( g_modules[ CLIENT_DLL ].find_pattern( "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0" ) ) );

	static const auto return_to_accumulate_layers = *( reinterpret_cast< void** >( g_modules[ CLIENT_DLL ].find_pattern( "84 C0 75 0D F6 87" ) ) );

	if ( const auto return_address = _ReturnAddress( ); return_address == return_to_setup_velocity || return_address == return_to_accumulate_layers )
		return true;

	return original( ecx );
}