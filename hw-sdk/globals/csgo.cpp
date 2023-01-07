#include "csgo.h"
#include "../dependencies/mocking_bird/mocking_bird.h"
#include "../utils/console/console.h"

bool csgo::init( )
{
	MOCKING_TRY;

	MOCK console::init( );

	console::print< console::log_level::SUCCESS >( _( "Hello world!" ) );

	MOCKING_CATCH( return false );

	return true;
}

bool csgo::unload( )
{
	MOCKING_TRY;

	MOCK console::unload( );

	MOCKING_CATCH( return false );

	return true;
}
