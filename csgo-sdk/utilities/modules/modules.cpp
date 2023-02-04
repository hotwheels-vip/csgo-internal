#include "modules.h"
#include "../../globals/includes/includes.h"
#include "../memory/structs/pe32.h"

#include <unordered_map>

std::unordered_map< unsigned int, module_t > dumped_modules = { };

address_t module_t::find( const char* signature )
{
	static const auto pattern_to_byte = []( const char* pattern ) {
		std::vector< int > bytes = { };

		const auto start = const_cast< char* >( pattern );
		const auto end   = const_cast< char* >( pattern ) + std::strlen( pattern );

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
	const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( reinterpret_cast< std::uint8_t* >( this->m_value ) + dos_header->e_lfanew );

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte( signature );
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
			return address_t( &scan_bytes[ i ] );
	}

	g_console.print( std::vformat( "failed to find pattern {:s}", std::make_format_args( signature ) ).c_str( ) );

	return address_t( nullptr );
}

bool n_modules::impl_t::on_attach( )
{
	const _PEB32* peb_data = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) );

	for ( LIST_ENTRY* list_entry = peb_data->Ldr->InLoadOrderModuleList.Flink; list_entry != &peb_data->Ldr->InLoadOrderModuleList;
	      list_entry             = list_entry->Flink ) {
		const _LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD( list_entry, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

		if ( entry->BaseDllName.Buffer ) {
			const auto converted_name = std::wstring( entry->BaseDllName.Buffer );

			if ( !( converted_name.empty( ) ) )
				dumped_modules[ HASH_RT( std::string( converted_name.begin( ), converted_name.end( ) ).c_str( ) ) ] = module_t( entry->DllBase );
		}
	}

	return !( dumped_modules.empty( ) );
}

module_t n_modules::impl_t::operator[]( unsigned int hash )
{
	return dumped_modules[ hash ];
}