#pragma once
#include <array>

class c_base_entity;
struct IDirect3DTexture9;

namespace n_avatar_cache
{
	struct impl_t {
		IDirect3DTexture9* operator[]( const int index );

		void on_add_entity( c_base_entity* entity );
		void on_remove_entity( c_base_entity* entity );
		void on_level_init_pre_entity( );
		void on_level_shutdown( );

	private:
		std::array< IDirect3DTexture9*, 64 > m_cached_avatars = { };
	};
} // namespace n_avatar_cache

inline n_avatar_cache::impl_t g_avatar_cache{ };