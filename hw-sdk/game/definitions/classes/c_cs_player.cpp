#include "../../sdk/classes/c_cs_player.h"

const bool sdk::c_cs_player::is_enemy( sdk::c_cs_player* player )
{
	static auto mp_teammates_are_enemies = g_convars[ _( "mp_teammates_are_enemies" ) ];

	if ( mp_teammates_are_enemies->get_bool( ) )
		return true;

	if ( this->team_number( ) == player->team_number( ) )
		return false;

	return true;
}

bool sdk::c_cs_player::is_alive( )
{
	return ( this->life_state( ) == sdk::life_state::LIFE_ALIVE && this->health( ) > 0 );
}

std::string sdk::c_cs_player::name( )
{
	if ( sdk::player_info_t info{ }; g_interfaces.engine->get_player_info( this->entity_index( ), &info ) )
		return std::string( info.name );

	return { };
}

void sdk::c_cs_player::update_client_side_animation( )
{
	if ( !this )
		return;

	using update_client_side_animation_type = void( __thiscall* )( c_cs_player* );

	static auto update_client_side_animation_address =
		g_client_dll.pattern_scan( _( "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36" ) ).as< std::uintptr_t >( );

	if ( !update_client_side_animation_address )
		return;

	reinterpret_cast< update_client_side_animation_type >( update_client_side_animation_address )( this );
}

void sdk::c_cs_player::set_local_view_angles( const math::vec3& view_angles )
{
	return virtual_func::call< void, const math::vec3& >( this, 373, view_angles );
}
