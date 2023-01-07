#include <string.h>

#include "../../utils/modules/modules.h"
#include "interfaces.h"

bool sdk::interfaces::init( )
{
	MOCKING_TRY;

	// find interfaces
	{
		engine = g_engine_dll.find_interface< sdk::iv_engine_client* >( VENGINE_CLIENT_INTERFACE_VERSION );

		client = g_client_dll.find_interface< sdk::i_client_dll* >( VCLIENT_INTERFACE_VERSION );

		entity_list = g_client_dll.find_interface< sdk::i_client_entity_list* >( VCLIENTENTITYLIST_INTERFACE_VERSION );

		prediction = g_client_dll.find_interface< sdk::i_prediction* >( VCLIENTPREDICTION_INTERFACE_VERSION );

		game_movement = g_client_dll.find_interface< sdk::i_game_movement* >( VCLIENTGAMEMOVEMENT_INTERFACE_VERSION );

		convar = g_vstdlib_dll.find_interface< sdk::i_cvar* >( CCONVAR_INTERFACE_VERSION );

		engine_trace = g_engine_dll.find_interface< sdk::i_engine_trace_client* >( VENGINE_CLIENT_TRACE_VERSION );

		engine_sound = g_engine_dll.find_interface< sdk::i_engine_sound* >( VENGINE_SOUND_CLIENT_VERSION );

		studio_render = g_studio_render_dll.find_interface< sdk::c_studio_render* >( VSTUDIO_RENDER_VERSION );

		debug_overlay = g_engine_dll.find_interface< sdk::i_debug_overlay* >( VENGINE_DEBUG_OVERLAY_VERSION );

		material_system = g_material_system_dll.find_interface< sdk::i_material_system* >( VMATERIAL_SYSTEM_CLIENT_VERSION );

		panel = g_vgui2_dll.find_interface< sdk::i_panel* >( VGUI_PANEL_CLIENT_VERSION );

		localize = g_localize_dll.find_interface< sdk::i_localize* >( LOCALIZE_LOCALIZE_VERSION );

		model_render = g_engine_dll.find_interface< sdk::iv_model_render* >( VENGINE_MODEL_VERSION );

		model_info = g_engine_dll.find_interface< sdk::iv_model_info* >( VENGINE_MODEL_INFO );

		model_cache = g_data_cache_dll.find_interface< sdk::i_model_cache* >( VDATA_MODEL_CACHE_VERSION );

		file_system = g_file_system_stdio_dll.find_interface< void* >( VFILE_SYSTEM_VERSION );
	}

	// virtuals

	globals = virtual_func::get< address >( client, 11 ).add( 0xA ).get< sdk::i_global_vars* >( 2 );

	// patterns

	device = **g_shaderapidx9_dll.pattern_scan( _( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) ).add( 0x1 ).as< IDirect3DDevice9*** >( );

	input = *g_client_dll.pattern_scan( _( "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) ).add( 0x1 ).as< sdk::c_input** >( );

	move_helper = **g_client_dll.pattern_scan( _( "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) ).add( 0x2 ).as< sdk::i_move_helper*** >( );

	client_state = **g_engine_dll.pattern_scan( _( "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) ).add( 0x1 ).as< sdk::c_client_state*** >( );

	key_values_system = reinterpret_cast< void*( __cdecl* )( ) >( LI_FN( GetProcAddress )( g_vstdlib_dll.module_handle, _( "KeyValuesSystem" ) ) )( );

	MOCKING_CATCH( return false );

	console::print< console::log_level::DEBUG >( _( "Initialized interfaces." ) );

	return true;
}
