#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

#include "../../hacks/entity_cache/entity_cache.h"
#include "../../hacks/visuals/edicts/edicts.h"

void __fastcall n_detoured_functions::frame_stage_notify( void* ecx, void* edx, int stage )
{
	static auto original = g_hooks.m_frame_stage_notify.get_original< decltype( &n_detoured_functions::frame_stage_notify ) >( );

	if ( !g_interfaces.m_engine_client->is_in_game( ) ) {
		g_edicts.reset( );
		return original( ecx, edx, stage );
	}

	if ( !g_ctx.m_local )
		return original( ecx, edx, stage );

	if ( !g_ctx.m_local->is_alive( ) )
		g_edicts.reset( );

	g_edicts.on_frame_stage_notify( stage );

	original( ecx, edx, stage );
}
