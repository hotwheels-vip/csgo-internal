#pragma once

#include <string_view>

struct utilities_t {
	void* get_module_handle( const std::string_view module_name );
	void* get_export_address( const void* module_handle, const std::string_view export_name );

	template< typename T = void* >
	constexpr T get_virtual_function( void* thisptr, std::size_t index )
	{
		return ( *static_cast< T** >( thisptr ) )[ index ];
	}

	inline std::uintptr_t get_absolute_address( const std::uintptr_t relative_address )
	{
		return relative_address + 0x4 + *reinterpret_cast< std::int32_t* >( relative_address );
	}

	const char8_t* get_weapon_icon( short item_definition_index );
};

inline utilities_t utilities = { };