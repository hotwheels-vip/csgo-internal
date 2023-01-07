#pragma once
#include <cstddef>

namespace virtual_func
{
	template< typename T, typename... VA >
	constexpr T call( void* thisptr, std::size_t index, VA... args )
	{
		using fn = T( __thiscall* )( void*, decltype( args )... );
		return ( *static_cast< fn** >( thisptr ) )[ index ]( thisptr, args... );
	}

	template< typename T = void* >
	constexpr T get( void* thisptr, std::size_t index )
	{
		return ( *static_cast< T** >( thisptr ) )[ index ];
	}
} // namespace virtual_func
