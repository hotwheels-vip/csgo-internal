#include "console.h"

#include "../../globals/includes/includes.h"

void n_console::impl_t::on_attach( const char* window_title )
{
#ifdef _DEBUG
	LI_FN( AllocConsole )( );

	freopen_s( reinterpret_cast< _iobuf** >( stdout ), "conout$", "w", static_cast< _iobuf* >( stdout ) );

	if ( window_title )
		LI_FN( SetConsoleTitleA )( window_title );
#endif
}

void n_console::impl_t::on_release( )
{
#ifdef _DEBUG
	fclose( static_cast< _iobuf* >( stdout ) );

	LI_FN( FreeConsole )( );
#endif
}
