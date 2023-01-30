#include "../../features/visuals/weather/weather.h"
#include "../hooks.h"

void* __fastcall n_detoured_functions::get_vcollide( void* ecx, void* edx, int model )
{
	static auto original = hooks.get_vcollide.get_original< decltype( &n_detoured_functions::get_vcollide ) >( );

	if ( model == -1 )
		return &weather.m_collide;

	return original( ecx, edx, model );
}