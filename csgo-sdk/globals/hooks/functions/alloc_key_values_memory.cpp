#include "../../includes/includes.h"
#include "../hooks.h"

void* __fastcall n_detoured_functions::alloc_key_values_memory( void* ecx, void* edx, int size )
{
	static auto original = g_hooks.m_alloc_key_values_memory.get_original< decltype( &n_detoured_functions::alloc_key_values_memory ) >( );

	static auto key_values_alloc_engine =
		g_modules[ HASH_CT( "engine.dll" ) ].find_pattern( "E8 ? ? ? ? 83 C4 08 84 C0 75 10 FF 75 0C" ).relative( ).add( 0x1 ).add( 0x4A ).cast<unsigned int>();

	static auto key_values_alloc_client = g_modules[ HASH_CT( "client.dll" ) ]
	                                          .find_pattern( "E8 ? ? ? ? 83 C4 08 84 C0 75 10" )
	                                          .relative( )
	                                          .add( 0x1 )
	                                          .add( 0x3E )
	                                          .cast< unsigned int >( );

		if ( const unsigned int return_address = reinterpret_cast< unsigned int >( _ReturnAddress( ) );
	     return_address == key_values_alloc_engine || return_address == key_values_alloc_client )
		return nullptr;

	return original( ecx, edx, size );
}