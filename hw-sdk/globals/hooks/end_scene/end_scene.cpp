#include <string.h>

#include "../../../hacks/features/lagcomp/lagcomp.h"
#include "../../../hacks/features/visuals/visuals.h"
#include "../../../hacks/features/indicators/indicators.h"
#include "../../../hacks/logging/logging.h"
#include "../../../hacks/menu/imgui/imgui_helper.h"
#include "../../../hacks/menu/menu.h"
#include "../../../utils/convars/convars.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../../ctx/ctx.h"
#include "../create_move/create_move.h"
#include "end_scene.h"

LONG __stdcall hooks::end_scene::end_scene_detour( IDirect3DDevice9* device )
{
	// static void* m_return = nullptr;
	//
	// if ( !m_return ) {
	//	MEMORY_BASIC_INFORMATION mem_info;
	//	VirtualQuery( _ReturnAddress( ), &mem_info, sizeof( MEMORY_BASIC_INFORMATION ) );
	//
	//	char mod_name[ MAX_PATH ]{ };
	//	GetModuleFileName( static_cast< HMODULE >( mem_info.AllocationBase ), mod_name, MAX_PATH );
	//
	//	if ( std::strstr( mod_name, GAMEOVERLAYRENDERER_DLL ) != nullptr )
	//		m_return = _ReturnAddress( );
	// }
	//
	// if ( m_return != _ReturnAddress( ) )
	//	return hooks::end_scene_hook.call_original< LONG >( device );

	static auto static_return = _ReturnAddress( );

	if ( static_return != _ReturnAddress( ) )
		return hooks::end_scene_hook.call_original< LONG >( device );

	if ( device ) {
		D3DVIEWPORT9 vp;
		device->GetViewport( &vp );

		g_ctx.screen_size.x = vp.Width;
		g_ctx.screen_size.y = vp.Height;
	}

	g_render.setup_state( );

	g_indicators.render( );

	g_visuals.render( );

	g_log.think( );

	if ( !g_menu.menu_initialised )
		g_imgui.menu_init( device );

	g_imgui.new_frame( );

	g_menu.draw( );

	g_imgui.end_frame( );

	g_render.finish_state( );

	return hooks::end_scene_hook.call_original< LONG >( device );
}