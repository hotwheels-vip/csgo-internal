#include "../hooks.h"
#include "../../features/entities/entities.h"
#include "../../features/entities/avatars.h"

void __stdcall n_detoured_functions::level_init_pre_entity(const char* map_name) {
	static auto original = hooks.level_init_pre_entity.get_original< decltype( &n_detoured_functions::level_init_pre_entity ) >( );

	entity_cache.on_level_init_pre_entity( );
	avatar_cache.on_level_init_pre_entity( );

	original( map_name );
}