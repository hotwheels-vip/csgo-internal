#pragma once
#include <unordered_map>

constexpr static std::uint32_t default_offset_basis = 0x811C9DC5;
constexpr static std::uint32_t prime                = 0x01000193;

namespace fnv_hashing
{
	inline std::unordered_map< std::uint32_t, std::string > hashes{ };

	template< typename s >
	struct fnv1a;

	template<>
	struct fnv1a< std::uint32_t > {
		constexpr static std::uint32_t hash( const std::string& string, const std::uint32_t val = default_offset_basis )
		{
			auto temp_hash      = ( string[ 0 ] == '\0' ) ? val : hash( &string[ 1 ], ( val ^ std::uint32_t( string[ 0 ] ) ) * prime );
			hashes[ temp_hash ] = string;

			return temp_hash;
		}

		constexpr static std::uint32_t hash( char const* string, const std::uint32_t val = default_offset_basis )
		{
			auto temp_hash      = ( string[ 0 ] == '\0' ) ? val : hash( &string[ 1 ], ( val ^ std::uint32_t( string[ 0 ] ) ) * prime );
			hashes[ temp_hash ] = string;

			return temp_hash;
		}
	};

	inline std::uint32_t fnv1a_rt( const char* key, const std::uint32_t len )
	{
		const char* data = ( char* )key;
		auto hash        = default_offset_basis;

		for ( std::size_t i = 0; i < len; ++i ) {
			const std::uint8_t val = static_cast< std::uint8_t >( data[ i ] );
			hash                   = hash ^ val;
			hash *= prime;
		}

		return hash;
	}

}; // namespace fnv_hashing
