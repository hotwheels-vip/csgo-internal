#include "../../features/entities/avatars.h"
#include "../../features/entities/entities.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::level_shutdown( void* thisptr )
{
	static auto original = hooks.level_shutdown.get_original< decltype( &n_detoured_functions::level_shutdown ) >( );

	entity_cache.on_level_shutdown( );
	avatar_cache.on_level_shutdown( );

	hooks.level_shutdown.get_original< decltype( &n_detoured_functions::level_shutdown ) >( )( thisptr );
}