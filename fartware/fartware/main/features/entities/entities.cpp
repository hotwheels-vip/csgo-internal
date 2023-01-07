#include "entities.h"

void entity_cache_t::on_add_entity( c_base_entity* entity )
{
	if ( !entity )
		return;

	const auto it_found = std::ranges::find( this->m_cached_players, entity );
	if ( it_found == this->m_cached_players.cend( ) && entity->index( ) >= 1 && entity->index( ) <= 64 )
		this->m_cached_players.push_back( entity );
}

void entity_cache_t::on_remove_entity( c_base_entity* entity )
{
	if ( !entity )
		return;

	const auto it_found = std::ranges::find( this->m_cached_players, entity );
	if ( it_found != this->m_cached_players.cend( ) )
		this->m_cached_players.erase( it_found );
}

void entity_cache_t::on_level_init_pre_entity( )
{
	if ( !this->m_cached_players.empty( ) )
		this->m_cached_players.clear( );
}

void entity_cache_t::on_level_shutdown( )
{
	if ( !this->m_cached_players.empty( ) )
		this->m_cached_players.clear( );
}

void entity_cache_t::enumerate( const std::function< void( c_base_entity* ) >& function ) const
{
	for ( const auto entity : this->m_cached_players ) {
		if ( entity )
			function( entity );
	}
}