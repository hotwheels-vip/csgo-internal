#include "avatars.h"
#include "../../globals.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../../utilities/render/render.h"
#include "entities.h"

void avatar_cache_t::on_frame_stage_notify( )
{
	entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
		if ( !entity )
			return;

		const int index = entity->index( );

		player_info_t player_info = { };
		if ( !interfaces.m_engine->get_player_info( index, &player_info ) )
			return;

		if ( this->m_cached_avatars[ index ] == nullptr )
			this->m_cached_avatars[ index ] = render.steam_image( CSteamID( ( uint64 )player_info.m_xuid ) );
	} );
}

void avatar_cache_t::on_level_init_pre_entity( )
{
	entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
		if ( !entity || entity->is_dormant() )
			return;

		const int index = entity->index( );

		player_info_t player_info = { };
		if ( !interfaces.m_engine->get_player_info( index, &player_info ) )
			return;

		this->m_cached_avatars[ index ] = nullptr;
	} );
}

void avatar_cache_t::on_level_shutdown( )
{
	entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
		if ( !entity || entity->is_dormant( ) )
			return;

		const int index = entity->index( );

		player_info_t player_info = { };
		if ( !interfaces.m_engine->get_player_info( index, &player_info ) )
			return;

		this->m_cached_avatars[ index ] = nullptr;
	} );
}