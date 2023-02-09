#include "../hooks.h"
#include "../../hacks/visuals/edicts/edicts.h"

void* __fastcall n_detoured_functions::get_vcollide(void* ecx, void* edx, int model_index) {
	static auto original = g_hooks.m_get_vcollide.get_original< decltype( &n_detoured_functions::get_vcollide ) >( );

	if ( model_index == -1 )
		return g_edicts.get_precipitation_collideable();

	return original( ecx, edx, model_index );
}