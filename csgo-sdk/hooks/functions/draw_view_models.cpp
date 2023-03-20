#pragma once
#include "../../hacks/visuals/screen/screen.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::draw_view_models( void* ecx, void* edx, c_view_setup& setup, bool draw_view_model, bool draw_scope_lens_mask )
{
	static auto original = g_hooks.m_draw_view_models.get_original< decltype( &n_detoured_functions::draw_view_models ) >( );

	g_screen.on_draw_view_models( setup );

	return original( ecx, edx, setup, draw_view_model, draw_scope_lens_mask );
}