#pragma once

#include <functional>
#include <vector>
#include <array>

#include "../../source_engine/classes/c_base_entity.h"

struct IDirect3DTexture9;

struct avatar_cache_t {
	void on_frame_stage_notify( );
	void on_level_init_pre_entity( );
	void on_level_shutdown( );

	IDirect3DTexture9* find( const int index )
	{
		return m_cached_avatars[ index ];
	}

private:
	std::array< IDirect3DTexture9*, 64 > m_cached_avatars = { };
};

inline avatar_cache_t avatar_cache = { };