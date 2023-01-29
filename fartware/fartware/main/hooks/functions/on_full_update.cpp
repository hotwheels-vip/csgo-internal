#include "../hooks.h"
#include "../../features/visuals/weather/weather.h"

void __fastcall n_detoured_functions::on_full_update( void* ecx, void* edx, const char* text )
{
	static auto original = hooks.on_full_update.get_original< decltype( &n_detoured_functions::on_full_update ) >( );

	original( ecx, edx, text );

	//if ( strstr( text, "forced immediate full update" ) ) 
	//	weather.m_timer = 5;
}