#include "modules.h"
#include "../../game/sdk/structs/interface_node_t.h"
#include "../../globals/includes/includes.h"
#include "../memory/structs/pe32.h"

#include <unordered_map>

std::unordered_map< unsigned int, module_t > modules = { };

/* todo ~ use a hash map for the interfaces, and module exports (initialise them upon injection) */

unsigned char* module_t::find_pattern( const char* signature )
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
	const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( reinterpret_cast< unsigned char* >( this->m_value ) + dos_header->e_lfanew );

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte( signature );
	const auto scan_bytes    = reinterpret_cast< unsigned char* >( this->m_value );

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

	g_console.print( std::vformat( "failed to find pattern {:s}", std::make_format_args( signature ) ).c_str( ) );

	return nullptr;
}

void* module_t::find_interface( const char* interface_name )
{
	std::string_view converted_interface_name = interface_name;
	if ( converted_interface_name.empty( ) )
		return nullptr;

	const auto get_register_list = [ & ]( ) -> interface_node_t* {
		const void* create_interface_export = this->find_export( "CreateInterface" );

		if ( !create_interface_export )
			throw std::runtime_error( "failed get CreateInterface address" );

		const std::uintptr_t create_interface_relative = reinterpret_cast< std::uintptr_t >( create_interface_export ) + 0x5;
		const std::uintptr_t create_interface = create_interface_relative + 4U + *reinterpret_cast< std::int32_t* >( create_interface_relative );
		return **reinterpret_cast< interface_node_t*** >( create_interface + 0x6 );
	};

	for ( const interface_node_t* register_data = get_register_list( ); register_data; register_data = register_data->m_next ) {
		if ( ( std::string_view( register_data->m_name ).compare( 0U, converted_interface_name.length( ), converted_interface_name ) == 0 &&
		       std::atoi( register_data->m_name + converted_interface_name.length( ) ) > 0 ) ||
		     converted_interface_name.compare( register_data->m_name ) == 0 ) {
			const auto interface_address = register_data->m_create_fn( );

			g_console.print(
				std::vformat( "found {:s} interface @ {:p}", std::make_format_args( register_data->m_name, interface_address ) ).c_str( ) );

			return interface_address;
		}
	}

	g_console.print( std::vformat( "failed to capture interface {:s}", std::make_format_args( interface_name ) ).c_str( ) );

	return nullptr;
}

void* module_t::find_export( const char* export_name )
{
	const std::string_view converted_export_name = export_name;
	if ( converted_export_name.empty( ) )
		return nullptr;

	const unsigned char* address                 = static_cast< const unsigned char* >( this->m_value );
	const IMAGE_DOS_HEADER* dos_header           = static_cast< const IMAGE_DOS_HEADER* >( this->m_value );
	const IMAGE_NT_HEADERS* nt_headers           = reinterpret_cast< const IMAGE_NT_HEADERS* >( address + dos_header->e_lfanew );
	const IMAGE_OPTIONAL_HEADER* optional_header = &nt_headers->OptionalHeader;

	const unsigned int export_size    = optional_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
	const unsigned int export_address = optional_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;

	if ( export_size <= 0U )
		return nullptr;

	const IMAGE_EXPORT_DIRECTORY* export_directory = reinterpret_cast< const IMAGE_EXPORT_DIRECTORY* >( address + export_address );
	const unsigned int* names_rva                  = reinterpret_cast< const unsigned int* >( address + export_directory->AddressOfNames );
	const unsigned int* functions_rva              = reinterpret_cast< const unsigned int* >( address + export_directory->AddressOfFunctions );
	const unsigned short* name_ordinals            = reinterpret_cast< const unsigned short* >( address + export_directory->AddressOfNameOrdinals );

	unsigned int right = export_directory->NumberOfNames;
	unsigned int left  = 0;

	while ( right != left ) {
		const unsigned int middle = left + ( ( right - left ) >> 1U );
		const int result          = converted_export_name.compare( reinterpret_cast< const char* >( address + names_rva[ middle ] ) );

		if ( result == 0 )
			return const_cast< void* >( static_cast< const void* >( address + functions_rva[ name_ordinals[ middle ] ] ) );

		if ( result > 0 )
			left = middle;
		else
			right = middle;
	}

	return nullptr;
}

bool n_modules::impl_t::on_attach( )
{
	const _PEB32* peb_data = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) );

	while ( modules.find( HASH_CT( "serverbrowser.dll" ) ) == modules.end( ) ) {
		for ( LIST_ENTRY* list_entry = peb_data->Ldr->InLoadOrderModuleList.Flink; list_entry != &peb_data->Ldr->InLoadOrderModuleList;
		      list_entry             = list_entry->Flink ) {
			const _LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD( list_entry, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

			if ( entry->BaseDllName.Buffer ) {
				const auto converted_name = std::wstring( entry->BaseDllName.Buffer );

				modules[ HASH_RT( std::string( converted_name.begin( ), converted_name.end( ) ).c_str( ) ) ] = module_t( entry->DllBase );
			}
		}
	}

	return !( modules.empty( ) );
}

module_t n_modules::impl_t::operator[]( unsigned int hash )
{
	return modules[ hash ];
}