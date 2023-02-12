#include "interfaces.h"

#include "../../globals/includes/includes.h"
#include "../../utilities/memory/virtual.h"

bool n_interfaces::impl_t::on_attach( )
{
	if ( !( m_base_client = static_cast< c_base_client_dll* >( g_modules[ CLIENT_DLL ].find_interface( "VClient018" ) ) ) )
		return false;

	if ( !( m_engine_client = static_cast< c_engine_client* >( g_modules[ ENGINE_DLL ].find_interface( "VEngineClient014" ) ) ) )
		return false;

	if ( !( m_client_entity_list = static_cast< c_client_entity_list* >( g_modules[ CLIENT_DLL ].find_interface( "VClientEntityList003" ) ) ) )
		return false;

	if ( !( m_input_system = static_cast< c_input_system* >( g_modules[ INPUTSYSTEM_DLL ].find_interface( "InputSystemVersion001" ) ) ) )
		return false;

	if ( !( m_material_system = static_cast< c_material_system* >( g_modules[ MATERIALSYSTEM_DLL ].find_interface( "VMaterialSystem080" ) ) ) )
		return false;

	if ( !( m_engine_trace = static_cast< c_engine_trace* >( g_modules[ ENGINE_DLL ].find_interface( "EngineTraceClient004" ) ) ) )
		return false;

	if ( !( m_debug_overlay = static_cast< c_debug_overlay* >( g_modules[ ENGINE_DLL ].find_interface( "VDebugOverlay004" ) ) ) )
		return false;

	if ( !( m_convar = static_cast< c_convar* >( g_modules[ VSTDLIB_DLL ].find_interface( "VEngineCvar007" ) ) ) )
		return false;

	if ( !( m_model_info = static_cast< c_model_info* >( g_modules[ ENGINE_DLL ].find_interface( "VModelInfoClient004" ) ) ) )
		return false;

	if ( !( m_game_movement = static_cast< c_game_movement* >( g_modules[ CLIENT_DLL ].find_interface( "GameMovement001" ) ) ) )
		return false;

	if ( !( m_prediction = static_cast< c_prediction* >( g_modules[ CLIENT_DLL ].find_interface( "VClientPrediction001" ) ) ) )
		return false;

	if ( !( m_model_render = static_cast< c_model_render* >( g_modules[ ENGINE_DLL ].find_interface( "VEngineModel016" ) ) ) )
		return false;

	if ( !( m_model_cache = static_cast< c_model_cache* >( g_modules[ DATACACHE_DLL ].find_interface( "MDLCache004" ) ) ) )
		return false;

	if ( !( m_engine_sound = g_modules[ ENGINE_DLL ].find_interface( "IEngineSoundClient003" ) ) )
		return false;

	if ( !( m_surface = static_cast< c_surface* >( g_modules[ VGUI_DLL ].find_interface( "VGUI_Surface031" ) ) ) )
		return false;

	if ( !( m_game_types = static_cast< c_game_types* >( g_modules[ MATCHMAKING_DLL ].find_interface( "VENGINE_GAMETYPES_VERSION002" ) ) ) )
		return false;

	if ( !( m_localize = static_cast< c_localize* >( g_modules[ LOCALIZE_DLL ].find_interface( "Localize_001" ) ) ) )
		return false;

	if ( !( m_panel = static_cast< c_panel* >( g_modules[ VGUI2_DLL ].find_interface( "VGUI_Panel009" ) ) ) )
		return false;

	if ( !( m_physics_collison = static_cast< c_physics_collison* >( g_modules[ PHYSICS_DLL ].find_interface( "VPhysicsCollision007" ) ) ) )
		return false;

	if ( !( m_studio_render = static_cast< c_studio_render* >( g_modules[ STUDIORENDER_DLL ].find_interface( "VStudioRender026" ) ) ) )
		return false;

	if ( !( m_key_values_system =
	            reinterpret_cast< c_key_values_system*( __cdecl* )( ) >( g_modules[ VSTDLIB_DLL ].find_export( HASH_BT( "KeyValuesSystem" ) ) )( ) ) )
		return false;
	else
		g_console.print(
			std::vformat( "found IKeyValuesSystem @ {:p}", std::make_format_args( reinterpret_cast< void* >( m_key_values_system ) ) ).c_str( ) );

	if ( !( m_global_vars_base = **reinterpret_cast< c_global_vars_base*** >( g_virtual.get< unsigned int >( m_base_client, 11 ) + 0xa ) ) )
		return false;
	else
		g_console.print(
			std::vformat( "found IGlobalVarsBase @ {:p}", std::make_format_args( reinterpret_cast< void* >( m_global_vars_base ) ) ).c_str( ) );

	if ( !( m_client_mode = **reinterpret_cast< void*** >( g_virtual.get< unsigned int >( m_base_client, 10 ) + 0x5 ) ) )
		return false;
	else
		g_console.print(
			std::vformat( "found IClientModeShared @ {:p}", std::make_format_args( reinterpret_cast< void* >( m_client_mode ) ) ).c_str( ) );

	if ( !( m_weapon_system =
	            *reinterpret_cast< c_weapon_system** >( g_modules[ CLIENT_DLL ].find_pattern( ( "8B 35 ? ? ? ? FF 10 0F B7 C0" ) ) + 0x2 ) ) )
		return false;
	else
		g_console.print(
			std::vformat( "found IWeaponSystem @ {:p}", std::make_format_args( reinterpret_cast< void* >( m_weapon_system ) ) ).c_str( ) );

	if ( !( m_client_state = **reinterpret_cast< c_client_state*** >(
				g_modules[ ENGINE_DLL ].find_pattern( ( "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) ) + 0x1 ) ) )
		return false;
	else
		g_console.print( std::vformat( "found IClientState @ {:p}", std::make_format_args( reinterpret_cast< void* >( m_client_state ) ) ).c_str( ) );

	if ( !( m_input = *reinterpret_cast< c_input** >( g_modules[ CLIENT_DLL ].find_pattern( "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) + 0x1 ) ) )
		return false;
	else
		g_console.print( std::vformat( "found IInput @ {:p}", std::make_format_args( reinterpret_cast< void* >( m_input ) ) ).c_str( ) );

	if ( !( m_direct_device =
	            **reinterpret_cast< IDirect3DDevice9*** >( g_modules[ SHADERAPIDX9_DLL ].find_pattern( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 0x1 ) ) )
		return false;
	else
		g_console.print(
			std::vformat( "found IDirect3DDevice9 @ {:p}", std::make_format_args( reinterpret_cast< void* >( m_direct_device ) ) ).c_str( ) );

	return true;
}
