#include "hooks.h"

bool hooks_t::on_attach( )
{
	constexpr auto initialise_hook = []( c_detour_hook& detour_class, void* function, void* detour, const char* hook_name ) {
		if ( !detour_class.create( function, detour ) ) {
			console.print( ( "failed to hook {:s} @ {:p}" ), hook_name, function );
			return false;
		} else {
			console.print( ( "{:s} hooked @ {:p}" ), hook_name, function );
			return true;
		}
	};

	try {
		const auto alloc_key_values_memory_address = utilities.get_virtual_function( memory.m_key_values_system, 2 );
		const auto create_move_address             = utilities.get_virtual_function( interfaces.m_client, 24 );
		const auto frame_stage_notify_address      = utilities.get_virtual_function( interfaces.m_client, 37 );
		const auto on_add_entity_address =
			reinterpret_cast< void* >( memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 51 8B 45 0C 53 56 8B F1 57" ) ) );
		const auto on_remove_entity_address = reinterpret_cast< void* >(
			memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 51 8B 45 0C 53 8B D9 56 57 83 F8 FF 75 07" ) ) );
		const auto level_init_pre_entity_address = utilities.get_virtual_function( interfaces.m_client, 5 );
		const auto level_shutdown_address        = utilities.get_virtual_function( interfaces.m_client, 7 );
		const auto paint_traverse_address        = utilities.get_virtual_function( interfaces.m_panel, 41 );
		const auto draw_set_color_address        = utilities.get_virtual_function( interfaces.m_surface, 15 );
		const auto lock_cursor_address           = utilities.get_virtual_function( interfaces.m_surface, 67 );
		const auto reset_address                 = utilities.get_virtual_function( memory.m_device, 16 );
		const auto end_scene_address             = utilities.get_virtual_function( memory.m_device, 42 );
		const auto vsnprint_address              = reinterpret_cast< void* >(
            memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 51 56 8B 75 0C 8D 45 14 57 8B 7D 08 8B D6 50 51 FF 75 10 8B CF E8 "
		                                                                            "? ? ? ? 83 C4 0C 85 C0 78 08 85 F6 7E 0C 3B C6 7C 08 8D 46 FF" ) ) );
		const auto draw_print_text_address      = utilities.get_virtual_function( interfaces.m_surface, 28 );
		const auto emit_sound_address           = utilities.get_virtual_function( interfaces.m_engine_sound, 5 );
		const auto override_mouse_input_address = utilities.get_virtual_function( memory.m_client_mode, 23 );

		const auto modify_eye_pos_address = reinterpret_cast< void* >(
			memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14 83 7F 60" ) ) );

		if ( MH_Initialize( ) != MH_OK ) {
			throw std::runtime_error( ( "failed initialize minhook" ) );
			return false;
		}

		initialise_hook( hooks.alloc_key_values_memory, alloc_key_values_memory_address, &n_detoured_functions::alloc_key_values_memory,
		                 ( "IKeyValuesSystem::AllocKeyValuesMemory()" ) );
		initialise_hook( hooks.create_move, create_move_address, &n_detoured_functions::create_move, ( "CHLClient::CreateMove()" ) );
		initialise_hook( hooks.frame_stage_notify, frame_stage_notify_address, &n_detoured_functions::frame_stage_notify,
		                 ( "CHLClient::FrameStageNotify()" ) );
		initialise_hook( hooks.on_add_entity, on_add_entity_address, &n_detoured_functions::on_add_entity, ( "IClientEntityList::OnAddEntity()" ) );
		initialise_hook( hooks.on_remove_entity, on_remove_entity_address, &n_detoured_functions::on_remove_entity,
		                 ( "IClientEntityList::OnRemoveEntity()" ) );
		initialise_hook( hooks.level_init_pre_entity, level_init_pre_entity_address, &n_detoured_functions::level_init_pre_entity,
		                 ( "CHLClient::LevelInitPreEntity()" ) );
		initialise_hook( hooks.level_shutdown, level_shutdown_address, &n_detoured_functions::level_shutdown, ( "CHLClient::LevelShutdown()" ) );
		initialise_hook( hooks.draw_set_color, draw_set_color_address, &n_detoured_functions::draw_set_color, ( "ISurface::DrawSetColor()" ) );
		initialise_hook( hooks.paint_traverse, paint_traverse_address, &n_detoured_functions::paint_traverse, ( "IPanel::PaintTraverse()" ) );
		initialise_hook( hooks.lock_cursor, lock_cursor_address, &n_detoured_functions::lock_cursor, ( "ISurface::LockCursor()" ) );
		initialise_hook( hooks.reset, reset_address, &n_detoured_functions::reset, ( "IDirect3DDevice9::Reset()" ) );
		initialise_hook( hooks.end_scene, end_scene_address, &n_detoured_functions::end_scene, ( "IDirect3DDevice9::EndScene()" ) );
		// initialise_hook( hooks.vsnprintf, vsnprint_address, &n_detoured_functions::vsnprintf, ( "vsnprintf" ) );
		// initialise_hook( hooks.draw_print_text, draw_print_text_address, &n_detoured_functions::draw_print_text, ( "ISurface::DrawPrintText()" ) );
		initialise_hook( hooks.emit_sound, emit_sound_address, &n_detoured_functions::emit_sound, ( "IEngineSound::EmitSound()" ) );
		initialise_hook( hooks.override_mouse_input, override_mouse_input_address, &n_detoured_functions::override_mouse_input,
		                 ( "IClientModeShared::OverrideMouseInput()" ) );
		initialise_hook( hooks.modify_eye_position, modify_eye_pos_address, &n_detoured_functions::modify_eye_position,
		                 ( "CBaseAnimating::ModifyEyePos()" ) );

		if ( interfaces.m_engine->is_in_game( ) ) {
			console.print( "force updated entity cache" );
			memory.m_client_state->m_delta_tick = -1;
		}

		return true;
	} catch ( const std::exception& ex ) {
		console.print( ( "{:s}" ), ex.what( ) );

#ifdef _DEBUG
		_RPT0( _CRT_ERROR, ex.what( ) );
#else
		std::abort( );
#endif
	}

	return false;
}