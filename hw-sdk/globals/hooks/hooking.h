#pragma once

#include <iostream>

#include "../../dependencies/minhook/include/MinHook.h"
#include "../../utils/console/console.h"

template< class T >
class hook_helper
{
private:
	void* source;
	void* original;

public:
	template< typename S, typename D >
	void create( S source, D destination, const char* name = _( "undefined" ) )
	{
		// this is pretty messy

		if ( MH_CreateHook( reinterpret_cast< void* >( source ), reinterpret_cast< void* >( destination ), &original ) != MH_OK )
			console::print< console::log_level::FATAL >( _( "Failed to create hook {} - [ {} -> {} ]" ), name, reinterpret_cast< void* >( source ),
			                                             reinterpret_cast< void* >( destination ) );

		if ( MH_EnableHook( reinterpret_cast< void* >( source ) ) != MH_OK )
			console::print< console::log_level::FATAL >( _( "Failed to enable hook {} - [ {} -> {} ]" ), name, reinterpret_cast< void* >( source ),
			                                             reinterpret_cast< void* >( destination ) );

		this->source = source;

		console::print< console::log_level::NORMAL >( _( "Hooked {} - [ {} -> {} ]" ), name, reinterpret_cast< void* >( source ),
		                                              reinterpret_cast< void* >( destination ) );
	}

	void disable( )
	{
		MH_DisableHook( source );
	}

	template< typename R, typename... A >
	R call_original( A... arguments )
	{
		return reinterpret_cast< T* >( original )( arguments... );
	}

	T* get_original( )
	{
		return reinterpret_cast< T* >( original );
	};
};
