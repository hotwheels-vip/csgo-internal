#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../hacks/entity_cache/entity_cache.h"

void __fastcall n_detoured_functions::level_shutdown( void* thisptr )
{
	static auto original = g_hooks.m_level_shutdown.get_original< decltype( &n_detoured_functions::level_shutdown ) >( );

	g_entity_cache.on_level_shutdown( );

	g_globals.local = nullptr;

	return original( thisptr );
}