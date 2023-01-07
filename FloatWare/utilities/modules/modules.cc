#include "modules.hh"
#include "../console/console.hh"
#include "../utilities.hh"

unsigned char* module_info_t::find_pattern( const char* signature )
{
	static auto pattern_to_byte = [ & ]( ) {
		std::vector< int > bytes = { };
		const auto start         = const_cast< char* >( signature );
		const auto end           = const_cast< char* >( signature ) + std::strlen( signature );

		for ( auto current = start; current < end; current++ ) {
			if ( *current == '?' ) {
				++current;

				if ( *current == '?' )
					++current;

				bytes.push_back( -1 );
			} else
				bytes.push_back( std::strtoul( current, &current, 16 ) );
		}

		return bytes;
	};

	const auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER >( this->m_value );
	const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( reinterpret_cast< unsigned char* >( this->m_value ) + dos_header->e_lfanew );

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte( );
	const auto scan_bytes    = reinterpret_cast< std::uint8_t* >( this->m_value );

	const auto s = pattern_bytes.size( );
	const auto d = pattern_bytes.data( );

	for ( auto i = 0ul; i < size_of_image - s; ++i ) {
		bool found = true;

		for ( auto j = 0ul; j < s; ++j ) {
			if ( scan_bytes[ i + j ] != d[ j ] && d[ j ] != -1 ) {
				found = false;
				break;
			}
		}

		if ( found )
			return &scan_bytes[ i ];
	}

	g_console.print( xs( "failed to find pattern {:s} in module {:s}" ), signature, this->m_name );

	return nullptr;
}

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