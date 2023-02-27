#include "hooks.h"

#include "../game/sdk/includes/includes.h"
#include "../globals/includes/includes.h"
#include "../utilities/memory/virtual.h"

bool n_hooks::impl_t::on_attach( )
{
	constexpr auto initialise_hook = []( c_detour_hook& detour_class, void* function, void* detour, const char* hook_name ) {
		if ( !detour_class.create( function, detour ) ) {
			g_console.print< n_console::log_level::WARNING >(
				std::vformat( "failed to hook {:s} @ {:p}", std::make_format_args( hook_name, function ) ).c_str( ) );
			return false;
		} else {
			g_console.print( std::vformat( "{:s} hooked @ {:p}", std::make_format_args( hook_name, function ) ).c_str( ) );
			return true;
		}
	};

	if ( MH_Initialize( ) != MH_OK ) {
		g_console.print< n_console::log_level::WARNING >( "failed to initialise minhook" );
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

	initialise_hook( m_paint_traverse, g_virtual.get( g_interfaces.m_panel, 41 ), &n_detoured_functions::paint_traverse, "IPanel::PaintTraverse()" );

	initialise_hook( m_on_add_entity, reinterpret_cast< void* >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 51 8B 45 0C 53 56 8B F1 57" ) ),
	                 &n_detoured_functions::on_add_entity, "IClientEntityList::OnAddEntity()" );

	initialise_hook( m_on_remove_entity,
	                 reinterpret_cast< void* >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 51 8B 45 0C 53 8B D9 56 57 83 F8 FF 75 07" ) ),
	                 &n_detoured_functions::on_remove_entity, "IClientEntityList::OnRemoveEntity()" );

	initialise_hook( m_level_init_pre_entity, g_virtual.get( g_interfaces.m_base_client, 5 ), &n_detoured_functions::level_init_pre_entity,
	                 "CHLClient::LevelInitPreEntity()" );

	initialise_hook( m_level_shutdown, g_virtual.get( g_interfaces.m_base_client, 7 ), &n_detoured_functions::level_shutdown,
	                 "CHLClient::LevelShutdown()" );

	initialise_hook( m_get_vcollide, g_virtual.get( g_interfaces.m_model_info, 6 ), &n_detoured_functions::get_vcollide,
	                 "CModelInfo::GetVCollide()" );

	initialise_hook( m_particle_collection_simulate,
	                 reinterpret_cast< void* >( g_modules[ CLIENT_DLL ].find_pattern(
						 "55 8B EC 83 E4 F8 83 EC 30 56 57 8B F9 0F 28 E1 8B 0D ? ? ? ? F3 0F 11 64 24 ? 89 7C 24 18 8B 81" ) ),
	                 &n_detoured_functions::particle_collection_simulate, "CParticleCollection::Simulate()" );

	initialise_hook( m_find_material, g_virtual.get( g_interfaces.m_material_system, 84 ), &n_detoured_functions::find_material,
	                 "CMaterialSystem::FindMaterial()" );

	initialise_hook( m_process_movement, g_virtual.get( g_interfaces.m_game_movement, 1 ), &n_detoured_functions::process_movement,
	                 "CGameMovement::ProcessMovement()" );

	initialise_hook( m_modify_eye_position, g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14" ),
	                 &n_detoured_functions::modify_eye_position, "CBaseAnimating::ModifyEyePos()" );

	initialise_hook( m_override_mouse_input, g_virtual.get( g_interfaces.m_client_mode, 23 ), &n_detoured_functions::override_mouse_input,
	                 ( "IClientModeShared::OverrideMouseInput()" ) );

	initialise_hook( m_glow_effect_spectator, g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 EC 14 53 8B 5D 0C 56 57 85 DB 74" ),
	                 &n_detoured_functions::glow_effect_spectator, "CCSPlayer::GlowEffectSpectator()" );

	initialise_hook( m_fire_event_intern, g_virtual.get( g_interfaces.m_game_event_manager, 9 ), &n_detoured_functions::fire_event_intern,
	                 "CGameEventManager::FireEventIntern()" );

	initialise_hook( m_lock_cursor, g_virtual.get( g_interfaces.m_surface, 67 ), &n_detoured_functions::lock_cursor, "ISurface::LockCursor()" );

	initialise_hook( m_reset, g_virtual.get( g_interfaces.m_direct_device, 16 ), &n_detoured_functions::reset, "IDirect3DDevice9::Reset()" );

	initialise_hook( m_level_init_post_entity, g_virtual.get( g_interfaces.m_base_client, 6 ), &n_detoured_functions::level_init_post_entity,
	                 "CHLClient::LevelInitPostEntity()" );

	initialise_hook( m_end_scene, g_virtual.get( g_interfaces.m_direct_device, 42 ), &n_detoured_functions::end_scene,
	                 "IDirect3DDevice9::EndScene()" );

	initialise_hook( m_draw_set_color, g_virtual.get( g_interfaces.m_surface, 15 ), &n_detoured_functions::draw_set_color,
	                 "ISurface::DrawSetColor()" );

	initialise_hook( m_draw_model_execute, g_virtual.get( g_interfaces.m_model_render, 21 ), &n_detoured_functions::draw_model_execute,
	                 "CModelRender::DrawModelExecute()" );

	initialise_hook( m_set_image_data_r8g8b8a8,
	                 g_modules[ PANORAMA_DLL ].find_pattern( "55 8B EC 83 E4 F8 81 ? ? ? ? ? 53 56 57 8B F9 8B ? ? ? ? ? 8B" ),
	                 &n_detoured_functions::set_image_data_r8g8b8a8, ( "CImageData::SetImageDataR8G8B8A8()" ) );

	initialise_hook( m_net_earliertempents, g_virtual.get( g_convars[ HASH_BT( "net_earliertempents" ) ], 13 ),
	                 &n_detoured_functions::net_earliertempents, ( "net_earliertempents::GetBool()" ) );

	initialise_hook( m_list_leaves_in_box, g_modules[ ENGINE_DLL ].find_pattern( "55 8B EC 83 EC ? 8B 4D ? 8D 55" ),
	                 &n_detoured_functions::list_leaves_in_box, "CEngineBSPTree::ListLeavesInBox()" );

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
