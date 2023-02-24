#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../hacks/entity_cache/entity_cache.h"
#include "../../hacks/misc/scaleform/scaleform.h"
#include "../../hacks/visuals/edicts/edicts.h"

void __stdcall n_detoured_functions::level_init_pre_entity( const char* map_name )
{
	static auto original = g_hooks.m_level_init_pre_entity.get_original< decltype( &n_detoured_functions::level_init_pre_entity ) >( );

	g_entity_cache.on_level_init_pre_entity( );
	g_edicts.reset( );

	const float rate = 1.f / g_interfaces.m_global_vars_base->m_interval_per_tick;
	g_convars[ HASH_BT( "cl_updaterate" ) ]->set_value( rate );
	g_convars[ HASH_BT( "cl_cmdrate" ) ]->set_value( rate );

	return original( map_name );
}
