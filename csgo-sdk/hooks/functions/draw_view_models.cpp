#pragma once
#include "../hooks.h"

void __fastcall n_detoured_functions::draw_view_models( void* ecx, void* edx, c_view_setup& setup, bool draw_view_model, bool draw_scope_lens_mask )
{
	static auto original = g_hooks.m_draw_view_models.get_original< decltype( &n_detoured_functions::draw_view_models ) >( );

	return original( ecx, edx, setup, draw_view_model, draw_scope_lens_mask );
}