#include "modules.h"

address modules::impl::pattern_scan( const char* sig )
{
	if ( !module_handle )
		console::print< console::log_level::FATAL >( _( "Failed to get handle - [ {} ]" ), module_name );

	const auto module_address   = reinterpret_cast< std::uint8_t* >( module_handle );
	const auto dos_header       = reinterpret_cast< PIMAGE_DOS_HEADER >( module_handle );
	const auto nt_headers       = reinterpret_cast< PIMAGE_NT_HEADERS >( module_address + dos_header->e_lfanew );
	const std::uintptr_t offset = find_pattern( module_address, nt_headers->OptionalHeader.SizeOfImage, sig );

	return offset;
};

std::uintptr_t modules::impl::find_pattern( std::uint8_t* region_start, std::uintptr_t region_size, const char* pattern )
{
	std::vector< int > vec_bytes = pattern_to_bytes( pattern );
	for ( unsigned long i = 0UL; i < region_size - vec_bytes.size( ); ++i ) {
		bool byte_found = true;
		for ( unsigned long s = 0UL; s < vec_bytes.size( ); ++s ) {
			if ( region_start[ i + s ] != vec_bytes[ s ] && vec_bytes[ s ] != -1 ) {
				byte_found = false;
				break;
			}
		}

		if ( byte_found )
			return reinterpret_cast< std::uintptr_t >( &region_start[ i ] );
	}
	console::print< console::log_level::FATAL >( _( "Find pattern failed - [ {} ]" ), pattern );
	return 0U;
}

std::vector< int > modules::impl::pattern_to_bytes( const char* pattern )
{
	std::vector< int > vec_bytes = { };
	char* start                  = const_cast< char* >( pattern );
	char* end                    = start + strlen( pattern );

	for ( char* current = start; current < end; ++current ) {
		if ( *current == '?' ) {
			++current;

			if ( *current == '?' )
				++current;

			vec_bytes.push_back( -1 );
		} else
			// convert byte to hex
			vec_bytes.push_back( strtoul( current, &current, 16 ) );
	}

	return vec_bytes;
}
constexpr HMODULE modules::impl::get_module_handle_safe( const char* name )
{
	HMODULE return_module{ };

	while ( !return_module ) {
		return_module = LI_FN( GetModuleHandleA )( name );
		utils::sleep( 1 );
	}

	return return_module;
}
