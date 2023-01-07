#pragma once

#include <limits>
#include <xutility>

namespace sdk
{

	template< typename t >
	struct bit_flag_t {
		bit_flag_t( ) = default;
		bit_flag_t( const t& value ) : m_value( value ){ };

		// flag has value
		inline bool has( const t& value ) const
		{
			return m_value & value;
		};

		inline bool has_any_of( std::initializer_list< t > lst )
		{
			return std::find( std::begin( lst ), std::end( lst ), m_value ) != std::end( lst );
		};

		// add to this flag a value
		inline void add( const t& value )
		{
			m_value |= value;
		};

		// remove from this flag a value
		inline void remove( const t& value )
		{
			m_value &= ~value;
		}

		// clear this flag
		inline void clear( )
		{
			m_value = { };
		};

		// nan check this flag
		inline bool is_empty( ) const
		{
			return m_value == std::numeric_limits< t >::quiet_NaN( );
		};

		// operator returns this flag
		inline operator t( ) const
		{
			return m_value;
		};

		// equality check operator
		constexpr bit_flag_t< t >& operator=( const bit_flag_t< t >& value )
		{
			m_value = value.m_value;

			return *this;
		};

		// equality check operator
		constexpr t& operator=( const t& value )
		{
			m_value = value;

			return m_value;
		}

	private:
		// flag value
		t m_value = { };
	};

	using int_flag_t = sdk::bit_flag_t< int >;
} // namespace sdk
