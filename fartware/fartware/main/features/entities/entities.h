#pragma once

#include <functional>
#include <vector>

#include "../../source_engine/classes/c_base_entity.h"

struct entity_cache_t {
	entity_cache_t( )
	{
		this->m_cached_players.reserve( 64 );
	}

	void on_add_entity( c_base_entity* entity );
	void on_remove_entity( c_base_entity* entity );
	void on_level_init_pre_entity( );
	void on_level_shutdown( );

	void enumerate( const std::function< void( c_base_entity* ) >& function ) const;

private:
	std::vector< c_base_entity* > m_cached_players = { };
};

inline entity_cache_t entity_cache = { };