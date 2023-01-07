#include "alloc_key_value_memory.h"

void* __fastcall hooks::alloc_key_value_memory::alloc_key_value_memory_detour( void* thisptr, int edx, int size )
{
	static const auto alloc_key_values_client = g_client_dll.pattern_scan( _( "FF 52 04 85 C0 74 0C 56" ) ).add( 0x3 ).as< uintptr_t >( );
	static const auto alloc_key_values_engine = g_engine_dll.pattern_scan( _( "FF 52 04 85 C0 74 0C 56" ) ).add( 0x3 ).as< uintptr_t >( );

	if ( const auto m_auto_return = reinterpret_cast< uintptr_t >( _ReturnAddress( ) );
	     m_auto_return == alloc_key_values_client || m_auto_return == alloc_key_values_engine )
		return nullptr;

	return hooks::alloc_key_value_memory_hook.call_original< void* >( thisptr, edx, size );
}
