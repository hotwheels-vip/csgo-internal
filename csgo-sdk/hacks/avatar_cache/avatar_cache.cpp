#include "avatar_cache.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

IDirect3DTexture9* n_avatar_cache::impl_t::operator[]( const int index )
{
	return this->m_cached_avatars[ index ];
}

void n_avatar_cache::impl_t::on_add_entity( c_base_entity* entity )
{
	if ( !entity )
		return;

	const int index = entity->get_index( );

	if ( !( index > 1 && index < 64 ) )
		return;

	player_info_t player_info = { };
	if ( !g_interfaces.m_engine_client->get_player_info( index, &player_info ) )
		return;

	if ( !( this->m_cached_avatars[ index ] ) )
		this->m_cached_avatars[ index ] = g_render.steam_image( CSteamID( ( uint64 )player_info.m_ull_xuid ) );
}

void n_avatar_cache::impl_t::on_remove_entity( c_base_entity* entity )
{
	if ( !entity )
		return;

	const int index = entity->get_index( );

	if ( !( index > 1 && index < 64 ) )
		return;

	player_info_t player_info = { };
	if ( !g_interfaces.m_engine_client->get_player_info( index, &player_info ) )
		return;

	if ( this->m_cached_avatars[ index ] != nullptr ) {
		this->m_cached_avatars[ index ]->Release( );
		this->m_cached_avatars[ index ] = nullptr;
	}
}

void n_avatar_cache::impl_t::reset( )
{
	for ( unsigned int iterator = 0; iterator < m_cached_avatars.size( ); iterator++ ) {
		if ( iterator < m_cached_avatars.size( ) ) {
			if ( this->m_cached_avatars[ iterator ] != nullptr ) {
				this->m_cached_avatars[ iterator ]->Release( );
				this->m_cached_avatars[ iterator ] = nullptr;
			}
		}
	}
}