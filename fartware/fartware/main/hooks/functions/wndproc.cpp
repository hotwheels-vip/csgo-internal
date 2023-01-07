#include "../hooks.h"

#include "../../menu/menu.h"

long __stdcall n_detoured_functions::wndproc( HWND window, UINT msg, WPARAM wide_param, LPARAM long_param )
{
	input.on_wndproc( msg, wide_param, long_param );

	if ( input.is_key_released( VK_INSERT ) )
		menu.m_opened = !menu.m_opened;

	interfaces.m_input_system->enable_input( !menu.m_opened );

	if ( render.m_initialised && menu.m_opened && ImGui_ImplWin32_WndProcHandler( window, msg, wide_param, long_param ) )
		return 1L;

	return CallWindowProcW( input.m_old_wnd_proc, window, msg, wide_param, long_param );
}