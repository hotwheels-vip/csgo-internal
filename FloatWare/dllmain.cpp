
#include <chrono>
#include <thread>
#include <windows.h>

#include "utilities/console.h"

static DWORD WINAPI on_attach( void* instance )
{
	g_console.on_attach( xs( "hotwheels | debug" ) );

	while ( !GetAsyncKeyState( VK_END ) && !GetAsyncKeyState( VK_DELETE ) )
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