#include "../../globals/includes/includes.h"
#include "../hooks.h"

int __fastcall n_detoured_functions::do_post_screen_effects( void* ecx, void* edx, int val )
{
	static auto original = g_hooks.m_do_post_screen_effects.get_original< int( __thiscall* )( void*, int ) >( );

	return original( g_interfaces.m_client_mode, val );
}