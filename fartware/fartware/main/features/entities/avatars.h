#pragma once

#include <functional>
#include <array>

#include "../../source_engine/classes/c_base_entity.h"

struct IDirect3DTexture9;

struct avatar_cache_t {
	void on_level_init_pre_entity( );
	void on_level_shutdown( );

	void on_add_entity( c_base_entity* entity );
	void on_remove_entity( c_base_entity* entity );

	IDirect3DTexture9* find( const int index )
	{
		return m_cached_avatars[ index ];
	}

private:
	std::array< IDirect3DTexture9*, 64 > m_cached_avatars = { };
};

inline avatar_cache_t avatar_cache = { };