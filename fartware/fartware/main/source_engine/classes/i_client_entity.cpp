#include "i_client_entity.h"
#include "../../memory/memory.h"

void i_client_entity::set_abs_origin( c_vector& origin )
{
	static auto set_abs_origin_address = memory.m_modules[ e_module_names::client ].find_pattern( ( ( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) ) );

	reinterpret_cast< void( __thiscall* )( void*, c_vector& ) >( set_abs_origin_address )( this, origin );
}
