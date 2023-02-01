#include "../../memory/memory.h"
#include "../hooks.h"

bool _fastcall n_detoured_functions::net_earliertempents( void* ecx, void* edx )
{
	static auto original = hooks.net_earliertempents.get_original< decltype( &n_detoured_functions::net_earliertempents ) >( );

	static auto cl_readpackets = reinterpret_cast<void*>(memory.m_modules[ e_module_names::engine ].find_pattern( "53 8A D9 8B 0D ? ? ? ? 56 57 8B B9 ? ? ? ? " ) );

	if ( _ReturnAddress( ) == cl_readpackets )
		return true;

	return original( ecx, edx );
}