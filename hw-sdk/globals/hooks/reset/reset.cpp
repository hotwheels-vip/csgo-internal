#include "reset.h"
#include "../../../hacks/menu/menu.h"
#include "../../../dependencies/imgui/dx9/imgui_impl_dx9.h"

long __stdcall hooks::reset::reset_detour( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters ) {
	static auto original = reset_hook.get_original( );

	if ( !g_menu.menu_initialised )
		return original( device, presentation_parameters );

	ImGui_ImplDX9_InvalidateDeviceObjects( );

	const HRESULT hreset = original( device, presentation_parameters );

	if ( hreset == D3D_OK )
		ImGui_ImplDX9_CreateDeviceObjects( );

	return hreset;
 }