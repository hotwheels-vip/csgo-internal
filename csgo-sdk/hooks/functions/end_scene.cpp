#include "../../globals/includes/includes.h"
#include "../../hacks/misc/misc.h"
#include "../hooks.h"

long __stdcall n_detoured_functions::end_scene( IDirect3DDevice9* device )
{
	static auto original      = g_hooks.m_end_scene.get_original< decltype( &n_detoured_functions::end_scene ) >( );
	static void* used_address = nullptr;

	if ( !used_address ) {
		MEMORY_BASIC_INFORMATION memory_basic_information = { };

		VirtualQuery( _ReturnAddress( ), &memory_basic_information, sizeof( MEMORY_BASIC_INFORMATION ) );

		char module_name[ MAX_PATH ] = { };
		GetModuleFileName( static_cast< HMODULE >( memory_basic_information.AllocationBase ), module_name, MAX_PATH );

		if ( strstr( module_name, ( "gameoverlayrenderer.dll" ) ) )
			used_address = _ReturnAddress( );
	}

	if ( _ReturnAddress( ) != used_address )
		return original( device );

	g_render.on_end_scene(
		[ & ]( ) {
			g_menu.on_end_scene( );
			g_misc.on_end_scene( );
		},
		device );

	return original( device );
}
