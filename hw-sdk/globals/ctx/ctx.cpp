#include "ctx.h"

bool ctx::impl::init( )
{
	MOCKING_TRY;

	// alloc backup mem
	if ( !g_ctx.backup.cmd )
		g_ctx.backup.cmd = reinterpret_cast< sdk::c_user_cmd* >( std::malloc( sizeof( sdk::c_user_cmd ) ) );
	if ( !g_ctx.backup.local )
		g_ctx.backup.local = reinterpret_cast< sdk::c_cs_player* >( std::malloc( 0x3870U ) );

	MOCKING_CATCH( return false );

	console::print< console::log_level::DEBUG >( _( "Initialized context." ) );

	return true;
}
