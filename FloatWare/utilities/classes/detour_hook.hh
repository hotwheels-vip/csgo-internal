#pragma once

#include <MinHook.h>

class c_detour_hook
{
public:
	c_detour_hook( ) = default;

	explicit c_detour_hook( void* function, void* detour ) : m_base_fn( function ), m_replace_fn( detour ) { }

	bool create( void* function, void* detour );
	bool replace( );

	template< typename fn >
	fn get_original( )
	{
		return static_cast< fn >( m_original_fn );
	}

private:
	bool m_is_hooked    = false;
	void* m_base_fn     = nullptr;
	void* m_replace_fn  = nullptr;
	void* m_original_fn = nullptr;
};