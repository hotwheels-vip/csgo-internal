#pragma once

#include <deque>

#include "../../../globals/includes/includes.h"
#include "../../../utils/renderer/renderer.h"

namespace config
{
	enum class variable_type : int {
		VARIABLE_BOOL,
		VARIABLE_INT,
		VARIABLE_FLOAT,
		VARIABLE_COLOR,
		VARIABLE_STRING,
		VARIABLE_VECTOR,
		VARIABLE_MAX
	};

	struct option {
		variable_type type{ };

		bool bool_value{ };

		int int_value{ };

		float float_value{ };

		float min_slider_value{ };
		float max_slider_value{ };

		color color_value{ };

		std::string string_value{ };

		std::vector< bool > vector_value{ };

		option( bool value )
		{
			type       = variable_type::VARIABLE_BOOL;
			bool_value = value;
		}

		option( int value, int min, int max )
		{
			type             = variable_type::VARIABLE_INT;
			int_value        = value;
			min_slider_value = static_cast< float >( min );
			max_slider_value = static_cast< float >( max );
		}

		option( float value, float min, float max )
		{
			type             = variable_type::VARIABLE_FLOAT;
			float_value      = value;
			min_slider_value = min;
			max_slider_value = max;
		}

		option( color value )
		{
			type        = variable_type::VARIABLE_COLOR;
			color_value = value;
		}

		option( std::string value )
		{
			type         = variable_type::VARIABLE_STRING;
			string_value = value;
		}

		option( std::vector< bool > value )
		{
			type         = variable_type::VARIABLE_VECTOR;
			vector_value = value;
		}
	};

	struct impl {
		std::unordered_map< std::int32_t, option > settings{ };

		bool init( );

		bool save( const std::string path );
		bool load( const std::string path );

		void insert( std::uint32_t hash, option _option );

		template< typename T >
		T& find( std::uint32_t hash )
		{
			auto found_option = settings.find( hash );

			if ( !found_option._Ptr )
				return *reinterpret_cast< T* >( nullptr );

			switch ( found_option->second.type ) {
			case variable_type::VARIABLE_BOOL:
				return ( T& )( found_option->second.bool_value );
			case variable_type::VARIABLE_INT:
				return ( T& )( found_option->second.int_value );
			case variable_type::VARIABLE_FLOAT:
				return ( T& )( found_option->second.float_value );
			case variable_type::VARIABLE_COLOR:
				return ( T& )( found_option->second.color_value );
			case variable_type::VARIABLE_STRING:
				return ( T& )( found_option->second.string_value );
			case variable_type::VARIABLE_VECTOR:
				return ( T& )( found_option->second.vector_value );
			}

#ifdef _DEBUG
			// console::print< console::log_level::FATAL >( "failed to find value in variable map [ hash: {} | name: {} ]", hash,
			//                                              fnv_hashing::hashes[ hash ].data( ); );
#endif

			return *reinterpret_cast< T* >( nullptr );
		}

		template< typename T >
		T& find( std::uint32_t hash, T& min, T& max )
		{
			auto found_option = settings.find( hash );

			if ( !found_option._Ptr )
				return *reinterpret_cast< T* >( nullptr );

			switch ( found_option->second.type ) {
			case variable_type::VARIABLE_INT:
				min = ( T )( found_option->second.min_slider_value );
				max = ( T )( found_option->second.max_slider_value );

				return ( T& )( found_option->second.int_value );
			case variable_type::VARIABLE_FLOAT:
				min = ( T )( found_option->second.min_slider_value );
				max = ( T )( found_option->second.max_slider_value );

				return ( T& )( found_option->second.float_value );
			}

#ifdef _DEBUG
			//console::print< console::log_level::FATAL >( "failed to find value in variable map [ hash: {} | name: {} ]", hash,
			//                                             fnv_hashing::hashes[ hash ].data( ); );
#endif

			return *reinterpret_cast< T* >( nullptr );
		}
	};
} // namespace config

inline config::impl g_config;
