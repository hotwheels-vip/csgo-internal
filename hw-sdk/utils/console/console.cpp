#include "console.h"
#include "../../dependencies/mocking_bird/mocking_bird.h"

bool console::init( )
{
#ifdef CONSOLE_ENABLED

	MOCKING_TRY;

	MOCK AllocConsole( );

	AttachConsole( ATTACH_PARENT_PROCESS );

	if ( freopen_s( reinterpret_cast< _iobuf** >( stdout ), _( "CONOUT$" ), _( "w" ), stdout ) != 0 )
		return false;

	if ( !SetConsoleTitleA( _( "> hotwheels" ) ) )
		return false;

	console::console_handle = GetStdHandle( STD_OUTPUT_HANDLE );
	MOCK console::console_handle;

	DWORD mode = 0;
	GetConsoleMode( console::console_handle, &mode );

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	return SetConsoleMode( console::console_handle, mode );

	MOCKING_CATCH( return false );
#endif
	return true;
}

void console::unload( )
{
#ifdef CONSOLE_ENABLED

	fclose( static_cast< _iobuf* >( stdout ) );

	FreeConsole( );

	if ( const auto console_window = GetConsoleWindow( ); console_window )
		PostMessageW( console_window, WM_CLOSE, 0U, 0L );
#endif
}
