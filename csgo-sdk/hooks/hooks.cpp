#include "hooks.h"

#include "../utilities/memory/virtual.h"
#include "../globals/includes/includes.h"

bool n_hooks::impl_t::on_attach( )
{
	constexpr auto initialise_hook = []( c_detour_hook& detour_class, void* function, void* detour, const char* hook_name ) {
		if ( !detour_class.create( function, detour ) ) {
			g_console.print( std::vformat( "failed to hook {:s} @ {:p}", std::make_format_args( hook_name, function ) ).c_str( ) );
			return false;
		} else {
			g_console.print( std::vformat( "{:s} hooked @ {:p}", std::make_format_args( hook_name, function ) ).c_str( ) );
			return true;
		}
	};

	if ( MH_Initialize( ) != MH_OK ) {
		g_console.print( "failed to initialise minhook" );
		return false;
	}

	initialise_hook( this->m_alloc_key_values_memory, g_virtual.get( g_interfaces.m_key_values_system, 2 ),
	                 &n_detoured_functions::alloc_key_values_memory, "IKeyValuesSystem::AllocKeyValuesMemory()" );

	initialise_hook( this->m_create_move_proxy, g_virtual.get( g_interfaces.m_base_client, 22 ), &n_detoured_functions::create_move_proxy,
	                 "CHLClient::CreateMove()" );

	initialise_hook( this->m_run_command, g_virtual.get( g_interfaces.m_prediction, 19 ), &n_detoured_functions::run_command,
	                 "IPrediction::RunCommand()" );

	initialise_hook( this->m_emit_sound, g_virtual.get( g_interfaces.m_engine_sound, 5 ), &n_detoured_functions::emit_sound,
	                 "IEngineSound::EmitSound()" );

	initialise_hook( this->m_frame_stage_notify, g_virtual.get( g_interfaces.m_base_client, 37 ), &n_detoured_functions::frame_stage_notify,
	                 "CHLClient::FrameStageNotify()" );

	initialise_hook( this->m_paint_traverse, g_virtual.get( g_interfaces.m_base_client, 37 ), &n_detoured_functions::paint_traverse,
	                 "IPanel::PaintTraverse()" );

	initialise_hook( this->m_lock_cursor, g_virtual.get( g_interfaces.m_surface, 67 ), &n_detoured_functions::lock_cursor, "ISurface::LockCursor()" );

	initialise_hook( this->m_reset, g_virtual.get( g_interfaces.m_direct_device, 16 ), &n_detoured_functions::reset, "IDirect3DDevice9::Reset()" );

	initialise_hook( this->m_end_scene, g_virtual.get( g_interfaces.m_direct_device, 42 ), &n_detoured_functions::end_scene,
	                 "IDirect3DDevice9::EndScene()" );

	return true;
}

void n_hooks::impl_t::on_release( )
{
	MH_DisableHook( MH_ALL_HOOKS );
	MH_RemoveHook( MH_ALL_HOOKS );
	MH_Uninitialize( );
}