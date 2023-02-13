#pragma once
#include <initializer_list>
#include <xutility>

namespace n_utilities
{
	struct impl_t {
		int create_thread( unsigned long __stdcall function( void* ), void* parameter );
		template< typename T >
		// is V inside lst, example of usage:
		// if (utils::is_in(num, {1, 2, 3})) { function }
		bool is_in( const T& v, std::initializer_list< T > lst )
		{
			return std::find( std::begin( lst ), std::end( lst ), v ) != std::end( lst );
		};
	};
} // namespace n_utilities

inline n_utilities::impl_t g_utilities;
