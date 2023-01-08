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
		while ( !utilities.get_module_handle( ( "serverbrowser.dll" ) ) )
			std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

		console.on_attach( ( "debug console" ) );

		const std::vector< const char* >& modules = { ( "client.dll" ),         ( "engine.dll" ),         ( "vgui2.dll" ),
			                                          ( "vguimatsurface.dll" ), ( "localize.dll" ),       ( "server.dll" ),
			                                          ( "panorama.dll" ),       ( "materialsystem.dll" ), ( "vstdlib.dll" ),
			                                          ( "shaderapidx9.dll" ),
			                                          ( "inputsystem.dll" ),
			                                          ( "steam_api.dll" ),
			                                          ( "datacache.dll" ) };

		console.print( ( "initialising module handles" ) );
		if ( !memory.on_attach( modules ) )
			console.print( ( "failed to initialise module handles" ) );
		else
			console.print( ( "initialised module handles" ) );

		console.print( ( "initialising interfaces" ) );
		if ( !interfaces.on_attach( ) )
			console.print( ( "failed to initialise interfaces" ) );
		else
			console.print( ( "initialised interfaces" ) );

		console.print( ( "initialising custom interfaces" ) );
		if ( !memory.on_attach( ) )
			console.print( ( "failed to initialise custom interfaces" ) );
		else
			console.print( ( "initialised custom interfaces" ) );

		console.print( ( "initialising netvars" ) );
		if ( !netvars.on_attach( ) )
			console.print( ( "failed to initialise netvars" ) );
		else
			console.print( ( "initialised netvars" ) );

		console.print( ( "initialising convars" ) );
		if ( !convars.on_attach( ) )
			console.print( ( "failed to initialise convars" ) );
		else
			console.print( ( "initialised convars" ) );

		console.print( ( "initialising input" ) );
		if ( !input.on_attach( ) )
			console.print( ( "failed to initialise input" ) );
		else
			console.print( ( "initialised input" ) );

		console.print( ( "initialising hooks" ) );
		if ( !hooks.on_attach( ) )
			console.print( ( "failed to initialise hooks" ) );
		else
			console.print( ( "initialised hooks" ) );

		console.print( ( "initialising configuration system" ) );
		if ( !config.on_attach( ) )
			console.print( ( "failed to initialise configuration system" ) );
		else
			console.print( ( "initialised configuration system" ) );

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