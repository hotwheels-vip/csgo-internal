#include "../../hacks/misc/scaleform/scaleform.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::level_init_post_entity( void* ecx, void* edx )
{
	static auto original = g_hooks.m_level_init_post_entity.get_original< decltype( &n_detoured_functions::level_init_post_entity ) >( );

	g_scaleform.on_level_init( );

	original( ecx, edx );
}
