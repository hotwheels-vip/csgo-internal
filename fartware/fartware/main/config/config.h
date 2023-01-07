#pragma once
#include <any>
#include <deque>
#include <filesystem>
#include <vector>

#include "../hashing/strings/fnv1a.h"
#include "../hashing/strings/xorstr.h"
#include "../source_engine/structs/key_bind_t.h"

template< typename T, std::size_t S >
inline std::vector< T > get_filled_vector( const T& fill )
{
	std::vector< T > temp( S );
	std::fill( temp.begin( ), temp.begin( ) + S, fill );
	return temp;
}

struct variable_object_t {
	variable_object_t( const unsigned int name_hash, const unsigned int type_hash, std::any&& default_value )
		: m_name_hash( name_hash ), m_type_hash( type_hash ), m_value( std::move( default_value ) )
	{
	}

	~variable_object_t( ) = default;

	template< typename T >
	T& get( )
	{
		return *static_cast< T* >( std::any_cast< T >( &m_value ) );
	}

	template< typename T >
	void set( T value )
	{
		m_value.emplace< T >( value );
	}

	unsigned int m_name_hash = 0x0;
	unsigned int m_type_hash = 0x0;
	std::any m_value         = { };
};

struct config_t {
	bool on_attach( );
	bool save( std::string_view file_name );
	bool load( std::string_view file_name );
	void remove( const std::size_t index );
	void refresh( );

	std::size_t get_variable_index( const unsigned int name_hash );

	const std::filesystem::path path             = std::string( xs( "settings" ) );
	std::deque< std::string > m_file_names       = { };
	std::vector< variable_object_t > m_variables = { };

	template< typename T >
	T& get( const std::uint32_t index )
	{
		return m_variables[ index ].get< T >( );
	}

	template< typename T >
	std::uint32_t add_new_variable( const unsigned int name_hash, const unsigned int type_hash, const T default_value )
	{
		m_variables.emplace_back( name_hash, type_hash, std::make_any< T >( default_value ) );
		return m_variables.size( ) - 1U;
	}
};

inline config_t config = { };

#define GET_CONFIG_BOOL( var )  config.get< bool >( var )
#define GET_CONFIG_INT( var )   config.get< int >( var )
#define GET_CONFIG_FLOAT( var ) config.get< float >( var )
#define GET_CONFIG_COLOR( var ) config.get< c_color >( var )
#define GET_CONFIG_BIND( var )  config.get< key_bind_t >( var )

#define TO_STRING( func ) #func
#define ADD_VARIABLE( variable_type, variable_name, default_value )                                                                                  \
	const std::uint32_t variable_name = config.add_new_variable< variable_type >( fnv1a::hash_const( TO_STRING( variable_name ) ),                   \
	                                                                              fnv1a::hash_const( TO_STRING( variable_type ) ), default_value );
#define ADD_VARIABLE_VECTOR( variable_type, vector_size, variable_name, default_value )                                                              \
	const std::uint32_t variable_name = config.add_new_variable< std::vector< variable_type > >(                                                     \
		fnv1a::hash_const( TO_STRING( variable_name ) ), fnv1a::hash_const( "std::vector<" TO_STRING( variable_type ) ">" ),                         \
		get_filled_vector< variable_type, vector_size >( default_value ) );

#define INVALID_VARIABLE ( std::size_t )( -1 )