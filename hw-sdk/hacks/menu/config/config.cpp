#include "config.h"
#include "../../../dependencies/json/json.hpp"
#include "../../../globals/macros/macros.h"
#include "../../logging/logging.h"

bool config::impl::init( )
{
	g_config.insert( HASH_BT( "m_menu_color" ), config::option( color( 155, 255, 10 ) ) );

	/* movement */
	g_config.insert( HASH_BT( "m_bh" ), config::option( false ) );
	g_config.insert( HASH_BT( "m_auto_align" ), config::option( false ) );
	g_config.insert( HASH_BT( "m_jb" ), config::option( false ) );
	g_config.insert( HASH_BT( "m_jb_key" ), config::option( 0, 0, 256 ) );

	g_config.insert( HASH_BT( "m_ej" ), config::option( false ) );
	g_config.insert( HASH_BT( "m_ej_key" ), config::option( 0, 0, 256 ) );
	g_config.insert( HASH_BT( "m_lj" ), config::option( false ) );

	g_config.insert( HASH_BT( "m_mj" ), config::option( false ) );
	g_config.insert( HASH_BT( "m_mj_key" ), config::option( 0, 0, 256 ) );
	g_config.insert( HASH_BT( "m_fast_duck" ), config::option( false ) );

	/* indicators */
	g_config.insert( HASH_BT( "m_keybind_indicator" ), config::option( false ) );

	// std::vector< bool > keybinds = { };
	// g_config.insert( HASH_BT( "m_displayed_keybinds" ), config::option( std::vector< bool > ) );

	g_config.insert( HASH_BT( "m_velocity_indicator" ), config::option( false ) );
	g_config.insert( HASH_BT( "m_velocity_indicator_show_pre" ), config::option( false ) );
	g_config.insert( HASH_BT( "m_velocity_indicator_fade" ), config::option( false ) );

	g_config.insert( HASH_BT( "m_stamina_indicator" ), config::option( false ) );

	return true;
}

bool config::impl::save( const std::string path )
{
	if ( !path.length( ) )
		return false;

	nlohmann::json reader{ };

	for ( auto& setting : settings ) {
		switch ( setting.second.type ) {
		case variable_type::VARIABLE_BOOL:
			reader[ fnv_hashing::hashes[ setting.first ] ] = setting.second.bool_value;
			break;
		case variable_type::VARIABLE_INT:
			reader[ fnv_hashing::hashes[ setting.first ] ] = setting.second.int_value;
			break;

		case variable_type::VARIABLE_FLOAT:
			reader[ fnv_hashing::hashes[ setting.first ] ] = setting.second.float_value;
			break;

		case variable_type::VARIABLE_COLOR:
			reader[ fnv_hashing::hashes[ setting.first ] ] = { setting.second.color_value.r, setting.second.color_value.g,
				                                               setting.second.color_value.b, setting.second.color_value.a };
			break;

		case variable_type::VARIABLE_STRING:
			reader[ fnv_hashing::hashes[ setting.first ] ] = setting.second.string_value;
			break;
		}
	}

	std::ofstream stream( path );

	stream << std::setw( 4 ) << reader << std::endl;

	stream.close( );

	g_log.print( console::format( _( "saved cfg {}" ), path ), _( "[config]" ), 10.f );

	return true;
}

bool config::impl::load( const std::string path )
{
	if ( !path.length( ) )
		return false;

	nlohmann::json reader{ };

	std::ifstream stream( path );

	stream >> reader;

	stream.close( );

	for ( auto& setting : settings ) {
		switch ( setting.second.type ) {
		case variable_type::VARIABLE_BOOL:
			setting.second.bool_value = reader[ fnv_hashing::hashes[ setting.first ] ];
			break;
		case variable_type::VARIABLE_INT:
			setting.second.int_value = reader[ fnv_hashing::hashes[ setting.first ] ];
			break;

		case variable_type::VARIABLE_FLOAT:
			setting.second.float_value = reader[ fnv_hashing::hashes[ setting.first ] ];
			break;

		case variable_type::VARIABLE_COLOR:
			setting.second.color_value.r = reader[ fnv_hashing::hashes[ setting.first ] ][ 0 ];
			setting.second.color_value.g = reader[ fnv_hashing::hashes[ setting.first ] ][ 1 ];
			setting.second.color_value.b = reader[ fnv_hashing::hashes[ setting.first ] ][ 2 ];
			setting.second.color_value.a = reader[ fnv_hashing::hashes[ setting.first ] ][ 3 ];

			break;

		case variable_type::VARIABLE_STRING:
			setting.second.string_value = reader[ fnv_hashing::hashes[ setting.first ] ];

			break;
		}
	}

	g_log.print( console::format( _( "loaded cfg {}" ), path ), _( "[config]" ), 10.f );
	return true;
}

void config::impl::insert( std::uint32_t hash, option _option )
{
	settings.insert( std::make_pair( hash, _option ) );
}
