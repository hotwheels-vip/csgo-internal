#include <fstream>
#include <shlobj.h>

#include "../../dependencies/json/json.hpp"

#include "config.h"

#include "../includes.h"

bool config_t::on_attach( )
{
	if ( !std::filesystem::is_directory( this->path ) ) {
		std::filesystem::remove( this->path );
		if ( !std::filesystem::create_directories( this->path ) )
			return false;
	}

	this->refresh( );

	return true;
}

bool config_t::save( std::string_view file_name )
{
	std::filesystem::path file_path( file_name );
	if ( file_path.extension( ) != ( ".hw" ) )
		file_path.replace_extension( ( ".hw" ) );

	const std::string file = std::filesystem::path( this->path / file_path ).string( );
	nlohmann::json config  = { };

	try {
		for ( auto& variable : this->m_variables ) {
			nlohmann::json entry = { };

			entry[ ( "name-id" ) ] = variable.m_name_hash;
			entry[ ( "type-id" ) ] = variable.m_type_hash;

			switch ( variable.m_type_hash ) {
			case fnv1a::hash_const( "int" ): {
				entry[ ( "value" ) ] = variable.get< int >( );
				break;
			}
			case fnv1a::hash_const( "float" ): {
				entry[ ( "value" ) ] = variable.get< float >( );
				break;
			}
			case fnv1a::hash_const( "bool" ): {
				entry[ ( "value" ) ] = variable.get< bool >( );
				break;
			}
			case fnv1a::hash_const( "std::string" ): {
				entry[ ( "value" ) ] = variable.get< std::string >( );
				break;
			}
			case fnv1a::hash_const( "c_color" ): {
				const auto& color = variable.get< c_color >( );

				nlohmann::json sub = { };

				sub.push_back( color.get< e_color_type::color_type_r >( ) );
				sub.push_back( color.get< e_color_type::color_type_g >( ) );
				sub.push_back( color.get< e_color_type::color_type_b >( ) );
				sub.push_back( color.get< e_color_type::color_type_a >( ) );

				entry[ ( "value" ) ] = sub.dump( );
				break;
			}
			case fnv1a::hash_const( "key_bind_t" ): {
				const auto& bind = variable.get< key_bind_t >( );

				nlohmann::json sub = { };

				sub.push_back( bind.m_key );
				sub.push_back( bind.m_key_style );

				entry[ ( "value" ) ] = sub.dump( );
				break;
			}
			case fnv1a::hash_const( "c_chams_settings" ): {
				const auto& chams_settings = variable.get< c_chams_settings >( );

				nlohmann::json sub = { };

				sub.push_back( chams_settings.m_enable_visible );
				sub.push_back( chams_settings.m_visible_color.get< e_color_type::color_type_r >( ) );
				sub.push_back( chams_settings.m_visible_color.get< e_color_type::color_type_g >( ) );
				sub.push_back( chams_settings.m_visible_color.get< e_color_type::color_type_b >( ) );
				sub.push_back( chams_settings.m_visible_color.get< e_color_type::color_type_a >( ) );
				sub.push_back( chams_settings.m_enable_invisible );
				sub.push_back( chams_settings.m_invisible_color.get< e_color_type::color_type_r >( ) );
				sub.push_back( chams_settings.m_invisible_color.get< e_color_type::color_type_g >( ) );
				sub.push_back( chams_settings.m_invisible_color.get< e_color_type::color_type_b >( ) );
				sub.push_back( chams_settings.m_invisible_color.get< e_color_type::color_type_a >( ) );
				sub.push_back( chams_settings.m_render_original_model );

				entry[ ( "value" ) ] = sub.dump( );
				break;
			}
			case fnv1a::hash_const( "std::vector<bool>" ): {
				const auto& booleans = variable.get< std::vector< bool > >( );

				nlohmann::json sub = { };

				for ( const auto&& value : booleans )
					sub.push_back( static_cast< bool >( value ) );

				entry[ ( "value" ) ] = sub.dump( );
				break;
			}
			case fnv1a::hash_const( "std::vector<int>" ): {
				const auto& integers = variable.get< std::vector< int > >( );

				nlohmann::json sub = { };

				for ( const auto& value : integers )
					sub.push_back( value );

				entry[ ( "value" ) ] = sub.dump( );
				break;
			}
			case fnv1a::hash_const( "std::vector<float>" ): {
				const auto& floats = variable.get< std::vector< float > >( );

				nlohmann::json sub = { };

				for ( const auto& value : floats )
					sub.push_back( value );

				entry[ ( "value" ) ] = sub.dump( );
				break;
			}
			case fnv1a::hash_const( "std::vector<std::string>" ): {
				const auto& strings = variable.get< std::vector< std::string > >( );

				nlohmann::json sub = { };

				for ( const auto& value : strings )
					sub.push_back( value );

				entry[ ( "value" ) ] = sub.dump( );
				break;
			}
			default:
				break;
			}

			config.push_back( entry );
		}
	} catch ( const nlohmann::detail::exception& ex ) {
		console.print( ( "failed to save {}" ), ex.what( ) );
		return false;
	}

	std::ofstream output_file( file, std::ios::out | std::ios::trunc );
	if ( !output_file.good( ) )
		return false;

	try {
		output_file << config.dump( 4 );
		output_file.close( );
	} catch ( std::ofstream::failure& ex ) {
		console.print( ( "failed to save {}" ), ex.what( ) );
		return false;
	}

	return true;
}

bool config_t::load( std::string_view file_name )
{
	const std::string file = std::filesystem::path( this->path / file_name ).string( );
	nlohmann::json config  = { };

	std::ifstream input_file( file, std::ios::in );

	if ( !input_file.good( ) )
		return false;

	try {
		config = nlohmann::json::parse( input_file, nullptr, false );

		if ( config.is_discarded( ) )
			return false;

		input_file.close( );
	} catch ( std::ifstream::failure& ex ) {
		console.print( ( "failed to load {}" ), ex.what( ) );
		return false;
	}

	try {
		for ( const auto& variable : config ) {
			const unsigned int index = this->get_variable_index( variable[ ( "name-id" ) ].get< unsigned int >( ) );

			if ( index == INVALID_VARIABLE )
				continue;

			auto& entry = this->m_variables[ index ];

			switch ( variable[ ( "type-id" ) ].get< unsigned int >( ) ) {
			case fnv1a::hash_const( "bool" ): {
				entry.set< bool >( variable[ ( "value" ) ].get< bool >( ) );
				break;
			}
			case fnv1a::hash_const( "float" ): {
				entry.set< float >( variable[ ( "value" ) ].get< float >( ) );
				break;
			}
			case fnv1a::hash_const( "int" ): {
				entry.set< int >( variable[ ( "value" ) ].get< int >( ) );
				break;
			}
			case fnv1a::hash_const( "std::string" ): {
				entry.set< std::string >( variable[ ( "value" ) ].get< std::string >( ) );
				break;
			}
			case fnv1a::hash_const( "c_color" ): {
				const nlohmann::json vector = nlohmann::json::parse( variable[ ( "value" ) ].get< std::string >( ) );

				entry.set< c_color >( c_color( vector[ 0 ].get< std::uint8_t >( ), vector[ 1 ].get< std::uint8_t >( ),
				                               vector[ 2 ].get< std::uint8_t >( ), vector[ 3 ].get< std::uint8_t >( ) ) );

				break;
			}
			case fnv1a::hash_const( "key_bind_t" ): {
				const nlohmann::json vector = nlohmann::json::parse( variable[ ( "value" ) ].get< std::string >( ) );

				entry.set< key_bind_t >( key_bind_t( vector[ 0 ].get< int >( ), vector[ 1 ].get< int >( ) ) );

				break;
			}

			case fnv1a::hash_const( "c_chams_settings" ): {
				const nlohmann::json vector = nlohmann::json::parse( variable[ ( "value" ) ].get< std::string >( ) );

				entry.set< c_chams_settings >(
					c_chams_settings( vector[ 0 ].get< bool >( ),
				                      c_color( vector[ 1 ].get< std::uint8_t >( ), vector[ 2 ].get< std::uint8_t >( ),
				                               vector[ 3 ].get< std::uint8_t >( ), vector[ 4 ].get< std::uint8_t >( ) ),
				                      vector[ 5 ].get< bool >( ),
				                      c_color( vector[ 6 ].get< std::uint8_t >( ), vector[ 7 ].get< std::uint8_t >( ),
				                               vector[ 8 ].get< std::uint8_t >( ), vector[ 9 ].get< std::uint8_t >( ) ) ) );

				break;
				break;
			}
			case fnv1a::hash_const( "std::vector<bool>" ): {
				const nlohmann::json vector = nlohmann::json::parse( variable[ ( "value" ) ].get< std::string >( ) );
				auto& booleans              = entry.get< std::vector< bool > >( );

				for ( std::size_t i = 0U; i < vector.size( ); i++ ) {
					if ( i < booleans.size( ) )
						booleans[ i ] = vector[ i ].get< bool >( );
				}

				break;
			}
			case fnv1a::hash_const( "std::vector<int>" ): {
				const nlohmann::json vector = nlohmann::json::parse( variable[ ( "value" ) ].get< std::string >( ) );
				auto& integers              = entry.get< std::vector< int > >( );

				for ( std::size_t i = 0U; i < vector.size( ); i++ ) {
					if ( i < integers.size( ) )
						integers[ i ] = vector[ i ].get< int >( );
				}

				break;
			}
			case fnv1a::hash_const( "std::vector<float>" ): {
				const nlohmann::json vector = nlohmann::json::parse( variable[ ( "value" ) ].get< std::string >( ) );
				auto& floats                = entry.get< std::vector< float > >( );

				for ( std::size_t i = 0U; i < vector.size( ); i++ ) {
					if ( i < floats.size( ) )
						floats[ i ] = vector[ i ].get< float >( );
				}

				break;
			}
			case fnv1a::hash_const( "std::vector<std::string>" ): {
				const nlohmann::json vector = nlohmann::json::parse( variable[ ( "value" ) ].get< std::string >( ) );
				auto& strings               = entry.get< std::vector< std::string > >( );

				for ( std::size_t i = 0U; i < vector.size( ); i++ ) {
					if ( i < strings.size( ) )
						strings[ i ] = vector[ i ].get< std::string >( );
				}

				break;
			}
			default:
				break;
			}
		}
	} catch ( const nlohmann::detail::exception& ex ) {
		console.print( ( "json load failed {}" ), ex.what( ) );
		return false;
	}

	return true;
}

void config_t::remove( const std::size_t index )
{
	const std::string& file_name = this->m_file_names[ index ];
	const std::string file       = std::filesystem::path( this->path / file_name ).string( );

	if ( std::filesystem::remove( file ) )
		this->m_file_names.erase( this->m_file_names.cbegin( ) + static_cast< std::ptrdiff_t >( index ) );
}

void config_t::refresh( )
{
	this->m_file_names.clear( );

	for ( const auto& it : std::filesystem::directory_iterator( this->path ) )
		if ( it.path( ).filename( ).extension( ) == ( ".hw" ) )
			this->m_file_names.push_back( it.path( ).filename( ).string( ) );
}

std::size_t config_t::get_variable_index( const unsigned int name_hash )
{
	for ( unsigned int i = 0U; i < this->m_variables.size( ); i++ ) {
		if ( this->m_variables[ i ].m_name_hash == name_hash )
			return i;
	}

	return INVALID_VARIABLE;
}