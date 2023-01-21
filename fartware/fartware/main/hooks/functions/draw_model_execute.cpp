#include "../../features/visuals/players/players.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::draw_model_execute( int ecx, int edx, void* context, void* state, model_render_info_t* info,
                                                          matrix3x4_t* bone_to_world )
{
	static auto original = hooks.draw_model_execute.get_original< decltype( &n_detoured_functions::draw_model_execute ) >( );

	if ( !interfaces.m_engine->is_in_game( ) )
		return original( ecx, edx, context, state, info, bone_to_world );

	if ( !globals.m_local )
		return original( ecx, edx, context, state, info, bone_to_world );

	if ( interfaces.m_model_render->is_forced_material_override( ) )
		return original( ecx, edx, context, state, info, bone_to_world );

	players.on_draw_model_execute( ecx, edx, context, state, info, bone_to_world );

	original( ecx, edx, context, state, info, bone_to_world );
	interfaces.m_model_render->forced_material_override( nullptr );
}