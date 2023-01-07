#pragma once

#include <cstddef>
#include <cstdint>

struct address {
protected:
	std::uintptr_t m_address{ };

public:
	constexpr address( ) : m_address{ 0 } { };

	address( std::uintptr_t ad ) : m_address{ ad } { };

	address( void* address ) : m_address{ reinterpret_cast< std::uintptr_t >( address ) } { };

	operator std::uintptr_t( ) const
	{
		return m_address;
	}

	operator void*( ) const
	{
		return reinterpret_cast< void* >( m_address );
	}

	template< typename T = address >
	T to( ) const
	{
		return *( T* )( m_address );
	}

	template< typename T = address >
	T as( ) const
	{
		return m_address ? ( T )( m_address ) : T( );
	}

	template< typename T = address >
	T at( std::ptrdiff_t offset ) const
	{
		return m_address ? *( T* )( m_address + offset ) : T( );
	}

	template< typename T = address >
	T add( std::ptrdiff_t offset ) const
	{
		return m_address ? ( T )( m_address + offset ) : T( );
	}

	template< typename T = address >
	T sub( std::ptrdiff_t offset ) const
	{
		return m_address ? ( T )( m_address - offset ) : T( );
	}

	template< typename T = address >
	T get( std::uint8_t dereferences = 1 ) const
	{
		if ( !m_address )
			return T( );

		auto addr = m_address;

		while ( dereferences-- )
			if ( addr )
				addr = *reinterpret_cast< std::uintptr_t* >( addr );

		return ( T )( addr );
	}

	template< typename T = std::uintptr_t >
	void set( const T& value )
	{
		if ( !m_address )
			return;

		*( T* )( m_address ) = value;
	}

	template< typename T = address >
	T relative( std::ptrdiff_t offset = 0x1 ) const
	{
		if ( !m_address )
			return T( );

		const std::uintptr_t new_address = m_address + offset;

		const auto relative_offset = *reinterpret_cast< std::int32_t* >( new_address );
		if ( !relative_offset )
			return T( );

		return ( T )( new_address + sizeof( std::int32_t ) + relative_offset );
	}
};
