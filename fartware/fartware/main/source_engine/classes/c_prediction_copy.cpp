#include "c_prediction_copy.h"
#include "../../memory/memory.h"

int c_prediction_copy::transfer_data( const char* operation, int entindex, data_map_t* dmap )
{
	static auto original_transfer_data = reinterpret_cast< int( __thiscall* )( void*, const char*, int, data_map_t* ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 8B 45 10 53 56 8B F1 57" ) ) );

	return original_transfer_data( this, operation, entindex, dmap );
}