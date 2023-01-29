#include "avatars.h"
#include "../../globals.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../../utilities/render/render.h"
#include "entities.h"

#include <d3d9.h>

void avatar_cache_t::on_level_init_pre_entity( )
{
	for ( unsigned int i = 0; i < m_cached_avatars.size( ); i++ ) {
		if ( !( i < m_cached_avatars.size( ) ) )
			break;

		m_cached_avatars[ i ] = nullptr;
	}
}

void avatar_cache_t::on_level_shutdown( )
{
	for ( unsigned int i = 0; i < m_cached_avatars.size( ); i++ ) {
		if ( !( i < m_cached_avatars.size( ) ) )
			break;

		m_cached_avatars[ i ] = nullptr;
	}
}

void avatar_cache_t::on_add_entity( c_base_entity* entity )
{
	if ( !entity )
		return;

	const int index = entity->index( );

	if ( !( index >= 1 && index <= 64 ) )
		return;

	player_info_t player_info = { };
	if ( !interfaces.m_engine->get_player_info( index, &player_info ) )
		return;

	if ( this->m_cached_avatars[ index ] == nullptr )
		this->m_cached_avatars[ index ] = render.steam_image( CSteamID( ( uint64 )player_info.m_xuid ) );
}

void avatar_cache_t::on_remove_entity( c_base_entity* entity )
{
	if ( !entity )
		return;

	const int index = entity->index( );

		if ( !( index >= 1 && index <= 64 ) )
		return;

	player_info_t player_info = { };
	if ( !interfaces.m_engine->get_player_info( index, &player_info ) )
		return;

	if ( this->m_cached_avatars[ index ] != nullptr )
		this->m_cached_avatars[ index ] = nullptr;
}