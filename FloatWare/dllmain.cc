
#include <chrono>
#include <thread>

#include "utilities/console/console.hh"
#include "utilities/modules/modules.hh"
#include "utilities/utilities.hh"

/* pattern scanning example, Regards */
/* 
	static auto address = g_modules.get( idx )
                                       .find_pattern(xs("55 8B EC 51 56 8B 75 0C 8D 45 14 57 8B 7D 08 8B D6 50 51 FF 75 10 8B CF E8 "
	                                                      "? ? ? ? 83 C4 0C 85 C0 78 08 85 F6 7E 0C 3B C6 7C 08 8D 46 FF"));
*/

static DWORD WINAPI on_attach( void* instance )
{
	if ( !g_utilities.get_module_handle( xs( "serverbrowser.dll" ) ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

	g_console.on_attach( xs( "hotwheels | debug" ) );

	const std::vector< const char* >& modules = { xs( "client.dll" ), xs( "engine.dll" ) };

	g_console.print( xs( "initialising module handles" ) );
	if ( !g_modules.on_attach( modules ) )
		g_console.print( xs( "failed to initialise module handles" ) );
	else
		g_console.print( xs( "initialised module handles" ) );

	while ( !GetAsyncKeyState( VK_END ) && !GetAsyncKeyState( VK_DELETE ) ) /* TODO ~ replace with our own input system :3 */
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	g_console.on_release( );

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