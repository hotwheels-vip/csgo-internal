#include "includes.h"

#include "hooks/hooks.h"

#include "convars/convars.h"
#include "source_engine/netvars/netvars.h"

#include "features/misc/misc.h"
#include "features/visuals/players/players.h"
#include "utilities/event_listener/event_listener.h"

#include <chrono>
#include <thread>

/* TODO ~ float
 *	~ make a lambda for exception handling or something similar, so we can use it in hooks and other
 *  ~ lua (jit)
 */

/* He's a big chungus
He's a big chunky boi
Such a big bun, yes
We are so overjoyed
To have a big chunky boi
A big and wonderful chungus such as he
Oh such as he
He's a big chungus
He's a big chunky boi
Such a big bun, yes
We are so overjoyed
To have a big chunky boi
A big and wonderful chungus such as he
Oh, such as he
Huh
Buns come in all shapes and all sizes
This one has so many surprises
I've never seen a giant quite like him
There's no one like him
Nobody like chungus
Huh
Get the game for PS4
For a limited time
Huh
Don't miss what it has in store
You're running out of time
Play the game of the year
The game with that colossal boi
He's a big chungus
He's a big chunky boi
Such a big bun, yes
We are so overjoyed
To have a big chunky boi
A big and wonderful chungus such as he
Oh, such as he
Huh
Buns come in all shapes and all sizes
This one has so many surprises
I've never seen a giant quite like him
There's no one like him
Nobody like chungus
Huh */

static unsigned long WINAPI on_attach( void* instance )
{
	[ & ]( ) {
		while ( !utilities.get_module_handle( ( "serverbrowser.dll" ) ) )
			std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

		console.on_attach( ( "debug console" ) );

		const std::vector< const char* >& modules = { ( "client.dll" ),   ( "engine.dll" ),       ( "vgui2.dll" ),       ( "vguimatsurface.dll" ),
			                                          ( "localize.dll" ), ( "server.dll" ),       ( "panorama.dll" ),    ( "materialsystem.dll" ),
			                                          ( "vstdlib.dll" ),  ( "shaderapidx9.dll" ), ( "inputsystem.dll" ), ( "steam_api.dll" ),
			                                          ( "datacache.dll" ),   ( "vphysics.dll" ) };

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

		console.print( ( "initialising chams materials" ) );
		if ( !players.on_attach( ) )
			console.print( ( "failed to chams materials" ) );
		else
			console.print( ( "initialised chams materials" ) );

		console.print( ( "initialising input" ) );
		if ( !input.on_attach( ) )
			console.print( ( "failed to initialise input" ) );
		else
			console.print( ( "initialised input" ) );

		console.print( ( "initialising event listener" ) );
		if ( !event_listener.on_attach( { "player_hurt", "player_death" } ) )
			console.print( ( "failed to initialise event listener" ) );
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
		event_listener.on_release( );
		hooks.on_release( );
		input.on_release( );
		render.on_release( );
		console.on_release( );
	}( );

	LI_FN( FreeLibrary )( static_cast< HMODULE >( instance ) );

	return 0;
}

int __stdcall DllMain( void* instance, unsigned long call_reason, void* reserved )
{
	if ( call_reason == DLL_PROCESS_ATTACH ) {
		LI_FN( DisableThreadLibraryCalls )( reinterpret_cast< HMODULE >( instance ) );

		std::thread( on_attach, instance ).detach( );
	}

	return TRUE;
}
