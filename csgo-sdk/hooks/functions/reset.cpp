#include "../../globals/includes/includes.h"
#include "../hooks.h"

long __stdcall n_detoured_functions::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters )
{
	static auto original = g_hooks.m_reset.get_original< decltype( &n_detoured_functions::reset ) >( );

	if ( !g_render.m_initialised )
		return original( device, presentation_parameters );

	ImGui_ImplDX9_InvalidateDeviceObjects( );

	const long reset_result = original( device, presentation_parameters );

	if ( reset_result == ( ( long )0l ) )
		ImGui_ImplDX9_CreateDeviceObjects( );

	return reset_result;
}