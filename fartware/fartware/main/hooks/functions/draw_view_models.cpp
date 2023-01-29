#include "../../features/visuals/world/world.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::draw_view_models( void* thisptr, void* edx, c_view_setup& setup, bool draw_view_model,
                                                        bool draw_scope_lens_mask )
{
	static auto original = hooks.draw_view_models.get_original< decltype( &n_detoured_functions::draw_view_models ) >( );

	world.on_draw_view_models( setup );

	return original( thisptr, edx, setup, draw_view_model, draw_scope_lens_mask );
}