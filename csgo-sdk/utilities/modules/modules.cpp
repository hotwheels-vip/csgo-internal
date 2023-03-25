#include "modules.h"
#include "../../game/sdk/structs/interface_node_t.h"
#include "../../globals/includes/includes.h"
#include "../memory/structs/pe32.h"

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
	constexpr auto get_register_list = []( const void* create_interface_export ) -> interface_node_t* {
		if ( !create_interface_export ) {
			throw std::runtime_error( "failed get CreateInterface address" );
			return nullptr;
		}

		const unsigned int create_interface_relative = reinterpret_cast< unsigned int >( create_interface_export ) + 0x5;
		const unsigned int create_interface          = create_interface_relative + 4u + *reinterpret_cast< int* >( create_interface_relative );
		return **reinterpret_cast< interface_node_t*** >( create_interface + 0x6 );
	};

	for ( const interface_node_t* register_data = get_register_list( this->find_export( HASH_BT( "CreateInterface" ) ) ); register_data;
	      register_data                         = register_data->m_next ) {
		if ( HASH_RT( register_data->m_name ) == HASH_RT( interface_name ) ) {
			const auto interface_address = register_data->m_create_fn( );

			g_console.print(
				std::vformat( "found {:s} interface @ {:p}", std::make_format_args( register_data->m_name, interface_address ) ).c_str( ) );

			return interface_address;
		}
	}

	g_console.print( std::vformat( "failed to capture interface {:s}", std::make_format_args( interface_name ) ).c_str( ) );

	return nullptr;
}

void* module_t::find_export( unsigned int hash )
{
	const auto converted_value = reinterpret_cast< unsigned int >( this->m_value );

	auto dos_headers                       = reinterpret_cast< IMAGE_DOS_HEADER* >( this->m_value );
	auto nt_headers                        = reinterpret_cast< IMAGE_NT_HEADERS* >( converted_value + dos_headers->e_lfanew );
	IMAGE_OPTIONAL_HEADER* optional_header = &nt_headers->OptionalHeader;

	unsigned int exportdir_address = optional_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;

	if ( optional_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size <= 0U )
		return nullptr;

	auto export_directory = reinterpret_cast< IMAGE_EXPORT_DIRECTORY* >( converted_value + exportdir_address );
	auto names_rva        = reinterpret_cast< unsigned int* >( converted_value + export_directory->AddressOfNames );
	auto functions_rva    = reinterpret_cast< unsigned int* >( converted_value + export_directory->AddressOfFunctions );
	auto name_ordinals    = reinterpret_cast< unsigned short* >( converted_value + export_directory->AddressOfNameOrdinals );

	for ( unsigned int i = 0; i < export_directory->AddressOfFunctions; i++ ) {
		if ( HASH_RT( reinterpret_cast< const char* >( converted_value + names_rva[ i ] ) ) == hash )
			return reinterpret_cast< void* >( converted_value + functions_rva[ name_ordinals[ i ] ] );
	}

	return nullptr;
}

bool n_modules::impl_t::on_attach( )
{
	const _PEB32* peb = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) );

	while ( this->m_modules.find( SERVERBROWSER_DLL ) == this->m_modules.end( ) ) {
		for ( LIST_ENTRY* list_entry = peb->Ldr->InLoadOrderModuleList.Flink; list_entry != &peb->Ldr->InLoadOrderModuleList;
		      list_entry             = list_entry->Flink ) {
			const _LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD( list_entry, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

			if ( entry->BaseDllName.Buffer ) {
				const auto dll_buffer = std::wstring( entry->BaseDllName.Buffer );
				const std::string converted_dll_buffer( dll_buffer.begin( ), dll_buffer.end( ) );

				const auto converted_name = converted_dll_buffer.c_str( );

				this->m_modules[ HASH_RT( converted_name ) ] = module_t( entry->DllBase, converted_name );
			}
		}
	}

	return !( this->m_modules.empty( ) );
}

module_t n_modules::impl_t::operator[]( unsigned int hash )
{
	return this->m_modules[ hash ];
}
