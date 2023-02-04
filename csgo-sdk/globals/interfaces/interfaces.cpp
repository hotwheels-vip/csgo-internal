#include "interfaces.h"

#include "../../game/sdk/includes.h"
#include "../../globals/includes/includes.h"

bool n_interfaces::impl_t::on_attach( )
{
	if ( ( this->m_base_client = static_cast< c_base_client_dll* >( g_modules[ HASH_CT( "client.dll" ) ].find_interface( "VClient" ) ) ) == nullptr )
		return false;

	if ( ( this->m_engine_client = static_cast< c_engine_client* >( g_modules[ HASH_CT( "engine.dll" ) ].find_interface( "VEngineClient" ) ) ) ==
	     nullptr )
		return false;

	return true;
}