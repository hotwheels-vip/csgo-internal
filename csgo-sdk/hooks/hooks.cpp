#include "hooks.h"

#include "../game/sdk/includes/includes.h"
#include "../globals/includes/includes.h"
#include "../utilities/memory/virtual.h"

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

	initialise_hook( m_alloc_key_values_memory, g_virtual.get( g_interfaces.m_key_values_system, 2 ), &n_detoured_functions::alloc_key_values_memory,
	                 "IKeyValuesSystem::AllocKeyValuesMemory()" );

	initialise_hook( m_create_move_proxy, g_virtual.get( g_interfaces.m_base_client, 22 ), &n_detoured_functions::create_move_proxy,
	                 "CHLClient::CreateMove()" );

	initialise_hook( m_run_command, g_virtual.get( g_interfaces.m_prediction, 19 ), &n_detoured_functions::run_command, "IPrediction::RunCommand()" );

	initialise_hook( m_emit_sound, g_virtual.get( g_interfaces.m_engine_sound, 5 ), &n_detoured_functions::emit_sound, "IEngineSound::EmitSound()" );

	initialise_hook( m_frame_stage_notify, g_virtual.get( g_interfaces.m_base_client, 37 ), &n_detoured_functions::frame_stage_notify,
	                 "CHLClient::FrameStageNotify()" );

	initialise_hook( m_paint_traverse, g_virtual.get( g_interfaces.m_panel, 37 ), &n_detoured_functions::paint_traverse, "IPanel::PaintTraverse()" );

	initialise_hook( m_on_add_entity, reinterpret_cast< void* >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 51 8B 45 0C 53 56 8B F1 57" ) ),
	                 &n_detoured_functions::on_add_entity, "IClientEntityList::OnAddEntity()" );

	initialise_hook( this->m_on_remove_entity,
	                 reinterpret_cast< void* >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 51 8B 45 0C 53 8B D9 56 57 83 F8 FF 75 07" ) ),
	                 &n_detoured_functions::on_remove_entity, "IClientEntityList::OnRemoveEntity()" );

	initialise_hook( m_level_init_pre_entity, g_virtual.get( g_interfaces.m_base_client, 5 ), &n_detoured_functions::level_init_pre_entity,
	                 "CHLClient::LevelInitPreEntity()" );

	initialise_hook( m_level_shutdown, g_virtual.get( g_interfaces.m_base_client, 7 ), &n_detoured_functions::level_shutdown,
	                 "CHLClient::LevelShutdown()" );

	initialise_hook( m_lock_cursor, g_virtual.get( g_interfaces.m_surface, 67 ), &n_detoured_functions::lock_cursor, "ISurface::LockCursor()" );

	initialise_hook( m_reset, g_virtual.get( g_interfaces.m_direct_device, 16 ), &n_detoured_functions::reset, "IDirect3DDevice9::Reset()" );

	initialise_hook( m_end_scene, g_virtual.get( g_interfaces.m_direct_device, 42 ), &n_detoured_functions::end_scene,
	                 "IDirect3DDevice9::EndScene()" );

	if ( g_interfaces.m_engine_client->is_in_game( ) )
		g_interfaces.m_client_state->m_delta_tick = -1;

	return true;
}

void n_hooks::impl_t::on_release( )
{
	MH_DisableHook( MH_ALL_HOOKS );
	MH_RemoveHook( MH_ALL_HOOKS );
	MH_Uninitialize( );
}