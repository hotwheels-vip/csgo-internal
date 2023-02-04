#pragma once

struct address_t {
	address_t( ){ };
	address_t( unsigned char* value )
	{
		this->m_value = value;
	}

	// clang-format off
	template< typename t = address_t  >
	t cast( ) { return reinterpret_cast< t >( this->m_value );}

	template< typename t = address_t  >
	t scast( ) { return static_cast< t >( this->m_value ); }

	template< typename t = address_t  >
	t ccast( ) { return const_cast< t >( this->m_value ); }
	// clang-format on

	template< typename t = address_t >
	t relative( unsigned int offset = 0x1 )
	{
		unsigned int new_address = ( unsigned int )this->m_value + offset;
		int relative_offset      = *reinterpret_cast< int* >( new_address );

		if ( !relative_offset )
			return t( );

		return ( t )( new_address + sizeof( unsigned int ) + relative_offset );
	}

private:
	unsigned char* m_value{ };
};

struct module_t {
	module_t( ){ };
	module_t( void* value )
	{
		this->m_value = value;
	}

	address_t find( const char* signature );

	void* get_value( )
	{
		return this->m_value;
	}

private:
	void* m_value{ };
};

namespace n_modules
{
	struct impl_t {
		bool on_attach( );

		module_t operator[]( unsigned int hash );
	};
} // namespace n_modules

inline n_modules::impl_t g_modules{ };