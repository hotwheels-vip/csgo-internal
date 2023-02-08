#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../utilities/memory/relative.h"

void* __fastcall n_detoured_functions::alloc_key_values_memory( void* ecx, void* edx, int size )
{
	static auto original = g_hooks.m_alloc_key_values_memory.get_original< decltype( &n_detoured_functions::alloc_key_values_memory ) >( );

	static const auto key_values_alloc_engine = g_relative.get( reinterpret_cast< unsigned int >(
													g_modules[ ENGINE_DLL ].find_pattern( ( "E8 ? ? ? ? 83 C4 08 84 C0 75 10 FF 75 0C" ) ) + 0x1 ) ) +
	                                            0x4A;

	static const auto key_values_alloc_client =
		g_relative.get( reinterpret_cast< unsigned int >( g_modules[ CLIENT_DLL ].find_pattern( ( "E8 ? ? ? ? 83 C4 08 84 C0 75 10" ) ) + 0x1 ) ) +
		0x3E;

	if ( const unsigned int return_address = reinterpret_cast< unsigned int >( _ReturnAddress( ) );
	     return_address == key_values_alloc_engine || return_address == key_values_alloc_client )
		return nullptr;

	return original( ecx, edx, size );
}
