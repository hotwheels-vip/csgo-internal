#include "includes.h"

#include "hooks/hooks.h"

#include "convars/convars.h"
#include "source_engine/netvars/netvars.h"

#include "features/misc/misc.h"

#include <chrono>
#include <thread>

/* TODO ~ float
 *	~ make a lambada for exception handling or something similar, so we can use it in hooks and other
 *  ~ lua (jit)
 */

static DWORD WINAPI on_attach( void* instance )
{
	[ & ]( ) {
		while ( !utilities.get_module_handle( xs( "serverbrowser.dll" ) ) )
			std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

		console.on_attach( xs( "debug console" ) );

		const std::vector< const char* >& modules = { xs( "client.dll" ),         xs( "engine.dll" ),         xs( "vgui2.dll" ),
			                                          xs( "vguimatsurface.dll" ), xs( "localize.dll" ),       xs( "server.dll" ),
			                                          xs( "panorama.dll" ),       xs( "materialsystem.dll" ), xs( "vstdlib.dll" ),
			                                          xs( "shaderapidx9.dll" ),
			                                          xs( "inputsystem.dll" ),
			                                          xs( "steam_api.dll" ),
			                                          xs( "datacache.dll" ) };

		console.print( xs( "initialising module handles" ) );
		if ( !memory.on_attach( modules ) )
			console.print( xs( "failed to initialise module handles" ) );
		else
			console.print( xs( "initialised module handles" ) );

		console.print( xs( "initialising interfaces" ) );
		if ( !interfaces.on_attach( ) )
			console.print( xs( "failed to initialise interfaces" ) );
		else
			console.print( xs( "initialised interfaces" ) );

		console.print( xs( "initialising custom interfaces" ) );
		if ( !memory.on_attach( ) )
			console.print( xs( "failed to initialise custom interfaces" ) );
		else
			console.print( xs( "initialised custom interfaces" ) );

		console.print( xs( "initialising netvars" ) );
		if ( !netvars.on_attach( ) )
			console.print( xs( "failed to initialise netvars" ) );
		else
			console.print( xs( "initialised netvars" ) );

		console.print( xs( "initialising convars" ) );
		if ( !convars.on_attach( ) )
			console.print( xs( "failed to initialise convars" ) );
		else
			console.print( xs( "initialised convars" ) );

		console.print( xs( "initialising input" ) );
		if ( !input.on_attach( ) )
			console.print( xs( "failed to initialise input" ) );
		else
			console.print( xs( "initialised input" ) );

		console.print( xs( "initialising hooks" ) );
		if ( !hooks.on_attach( ) )
			console.print( xs( "failed to initialise hooks" ) );
		else
			console.print( xs( "initialised hooks" ) );

		console.print( xs( "initialising configuration system" ) );
		if ( !config.on_attach( ) )
			console.print( xs( "failed to initialise configuration system" ) );
		else
			console.print( xs( "initialised configuration system" ) );

		/* lua_State* L = lua_open( );
		luaL_openlibs( L );

		lua_pushstring( L, "float (goat)" );
		lua_setglobal( L, "username" );
		luaL_loadstring( L, "print(username)" );
		lua_pcall( L, 0, 0, 0 );

		lua_pushinteger( L, 15 );
		lua_setglobal( L, "age" );
		luaL_loadstring( L, "print(age)" );
		lua_pcall( L, 0, 0, 0 ); */
	}( );

	while ( !input.is_key_down( VK_END ) && !input.is_key_down( VK_DELETE ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	[ & ]( ) {
		hooks.on_release( );

		input.on_release( );

		render.on_release( );

		console.on_release( );
	}( );

	LI_FN( FreeLibraryAndExitThread )( static_cast< HMODULE >( instance ), 0 );

	return 0;
}

int __stdcall DllMain( void* instance, unsigned long call_reason, void* reserved )
{
	if ( call_reason == DLL_PROCESS_ATTACH ) {
		LI_FN( DisableThreadLibraryCalls )( reinterpret_cast< HMODULE >( instance ) );

		if ( const auto thread = LI_FN( CreateThread )( nullptr, 0, on_attach, instance, 0, nullptr ); thread )
			LI_FN( CloseHandle )( thread );
	}

	return TRUE;
}