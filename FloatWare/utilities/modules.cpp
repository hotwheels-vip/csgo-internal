#include "modules.h"
#include "console.h"
#include "utilities.h"

bool modules::impl::on_attach( const std::vector< const char* >& modules )
{
	for ( const auto& module_names : modules )
		this->m_modules.push_back( module_info_t( module_names, g_utilities.get_module_handle( module_names ) ) );

	for ( const auto& module_data : m_modules ) {
		if ( !module_data.m_name ) {
			g_console.print( xs( "failed to find {:s}" ), module_data.m_name );
			return false;
		}

		if ( !module_data.m_value ) {
			g_console.print( xs( "module handle {:s} was {:p}" ), module_data.m_name, module_data.m_value );
			return false;
		}

		g_console.print( xs( "found {:s} @ {:p}" ), module_data.m_name, module_data.m_value );
	}

	return true;
}