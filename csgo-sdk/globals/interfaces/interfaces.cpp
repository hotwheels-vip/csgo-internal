#include "interfaces.h"

#include "../../globals/includes/includes.h"

bool n_interfaces::impl_t::on_attach( )
{
	if ( ( this->m_base_client = static_cast< c_base_client_dll* >( g_modules[ HASH_CT( "client.dll" ) ].find_interface( "VClient" ) ) ) == nullptr )
		return false;

	if ( ( this->m_engine_client = static_cast< c_engine_client* >( g_modules[ HASH_CT( "engine.dll" ) ].find_interface( "VEngineClient" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_client_entity_list =
	           static_cast< c_client_entity_list* >( g_modules[ HASH_CT( "client.dll" ) ].find_interface( "VClientEntityList" ) ) ) == nullptr )
		return false;

	if ( ( this->m_input_system =
	           static_cast< c_input_system* >( g_modules[ HASH_CT( "inputsystem.dll" ) ].find_interface( "InputSystemVersion" ) ) ) == nullptr )
		return false;

	if ( ( this->m_key_values_system = reinterpret_cast< c_key_values_system*( __cdecl* )( ) >(
			   g_modules[ HASH_CT( "vstdlib.dll" ) ].find_export( "KeyValuesSystem" ) )( ) ) == nullptr )
		return false;
	else
		g_console.print(
			std::vformat( "found IKeyValuesSystem @ {:p}", std::make_format_args( reinterpret_cast< void* >( this->m_key_values_system ) ) )
				.c_str( ) );

	return true;
}