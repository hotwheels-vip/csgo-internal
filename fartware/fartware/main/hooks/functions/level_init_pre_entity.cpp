#include "../../convars/convars.h"
#include "../../features/entities/avatars.h"
#include "../../features/entities/entities.h"
#include "../../features/prediction/prediction.h"
#include "../hooks.h"

void __stdcall n_detoured_functions::level_init_pre_entity( const char* map_name )
{
	static auto original = hooks.level_init_pre_entity.get_original< decltype( &n_detoured_functions::level_init_pre_entity ) >( );

	prediction.m_last_cmd = nullptr;

	entity_cache.on_level_init_pre_entity( );
	avatar_cache.on_level_init_pre_entity( );

	float rate = 1.f / memory.m_globals->m_interval_per_tick;
	convars.find( fnv1a::hash( "cl_updaterate" ) )->set_value( rate );
	convars.find( fnv1a::hash( "cl_cmdrate" ) )->set_value( rate );

	return original( map_name );
}
