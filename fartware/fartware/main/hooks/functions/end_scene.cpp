#include "../hooks.h"

#include "../../features/misc/misc.h"
#include "../../features/visuals/players/players.h"
#include "../../menu/menu.h"

long __stdcall n_detoured_functions::end_scene( IDirect3DDevice9* device )
{
	static auto original      = hooks.end_scene.get_original< decltype( &n_detoured_functions::end_scene ) >( );
	static void* used_address = nullptr;

	if ( !used_address ) {
		MEMORY_BASIC_INFORMATION memory_basic_information = { };

		VirtualQuery( _ReturnAddress( ), &memory_basic_information, sizeof( MEMORY_BASIC_INFORMATION ) );

		char module_name[ MAX_PATH ] = { };
		GetModuleFileName( static_cast< HMODULE >( memory_basic_information.AllocationBase ), module_name, MAX_PATH );

		if ( strstr( module_name, ( "gameoverlayrenderer.dll" ) ) != nullptr )
			used_address = _ReturnAddress( );
	}

	if ( _ReturnAddress( ) != used_address )
		return original( device );

	render.on_end_scene(
		[ & ]( ) {
			menu.on_end_scene( );
			players.spectator_list( );
			// misc.practice.render( );
		},
		device );

	return original( device );
}
