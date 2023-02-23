#pragma once
#include <array>
#include <initializer_list>
#include <string>
#include <xutility>

namespace n_utilities
{
	struct impl_t {
		std::array< std::string, 8 > m_hit_groups = { "body", "head", "chest", "stomach", "left arm", "right arm", "left leg", "right leg" };

		bool is_weapon_valid( );

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
