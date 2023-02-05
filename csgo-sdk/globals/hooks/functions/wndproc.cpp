#include "../hooks.h"

#include "../../../globals/includes/includes.h"
#include "../../../game/sdk/includes/includes.h"

long __stdcall n_detoured_functions::wndproc( HWND window, unsigned int message, unsigned int wide_param, long long_param )
{
	g_input.on_wndproc( message, wide_param, long_param );

	if ( g_input.is_key_released( VK_INSERT ) )
		g_menu.m_opened = !g_menu.m_opened;

	g_interfaces.m_input_system->enable_input( !g_menu.m_opened );

	if ( g_render.m_initialised && g_menu.m_opened && ImGui_ImplWin32_WndProcHandler( window, message, wide_param, long_param ) )
		return 1L;

	return CallWindowProcW( g_input.m_old_wnd_proc, window, message, wide_param, long_param );
}