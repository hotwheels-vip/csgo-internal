#include "interfaces.h"

#include "../../globals/includes/includes.h"
#include "../../utilities/memory/virtual.h"

bool n_interfaces::impl_t::on_attach( )
{
	if ( ( this->m_base_client = static_cast< c_base_client_dll* >( g_modules[ HASH_CT( "client.dll" ) ].find_interface( "VClient018" ) ) ) == nullptr )
		return false;

	if ( ( this->m_engine_client = static_cast< c_engine_client* >( g_modules[ HASH_CT( "engine.dll" ) ].find_interface( "VEngineClient014" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_client_entity_list =
	           static_cast< c_client_entity_list* >( g_modules[ HASH_CT( "client.dll" ) ].find_interface( "VClientEntityList003" ) ) ) == nullptr )
		return false;

	if ( ( this->m_input_system =
	           static_cast< c_input_system* >( g_modules[ HASH_CT( "inputsystem.dll" ) ].find_interface( "InputSystemVersion001" ) ) ) == nullptr )
		return false;

	if ( ( this->m_material_system =
	           static_cast< c_material_system* >( g_modules[ HASH_CT( "materialsystem.dll" ) ].find_interface( "VMaterialSystem080" ) ) ) == nullptr )
		return false;

	if ( ( this->m_engine_trace = static_cast< c_engine_trace* >( g_modules[ HASH_CT( "engine.dll" ) ].find_interface( "EngineTraceClient004" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_debug_overlay = static_cast< c_debug_overlay* >( g_modules[ HASH_CT( "engine.dll" ) ].find_interface( "VDebugOverlay004" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_convar = static_cast< c_convar* >( g_modules[ HASH_CT( "vstdlib.dll" ) ].find_interface( "VEngineCvar007" ) ) ) == nullptr )
		return false;

	if ( ( this->m_model_info = static_cast< c_model_info* >( g_modules[ HASH_CT( "engine.dll" ) ].find_interface( "VModelInfoClient004" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_game_movement = static_cast< c_game_movement* >( g_modules[ HASH_CT( "client.dll" ) ].find_interface( "GameMovement001" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_prediction = static_cast< c_prediction* >( g_modules[ HASH_CT( "client.dll" ) ].find_interface( "VClientPrediction001" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_model_render = static_cast< c_model_render* >( g_modules[ HASH_CT( "engine.dll" ) ].find_interface( "VEngineModel016" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_model_cache = static_cast< c_model_cache* >( g_modules[ HASH_CT( "datacache.dll" ) ].find_interface( "MDLCache004" ) ) ) == nullptr )
		return false;

	if ( ( this->m_engine_sound = g_modules[ HASH_CT( "engine.dll" ) ].find_interface( "IEngineSoundClient003" ) ) == nullptr )
		return false;

	if ( ( this->m_surface = static_cast< c_surface* >( g_modules[ HASH_CT( "vguimatsurface.dll" ) ].find_interface( "VGUI_Surface031" ) ) ) == nullptr )
		return false;

	if ( ( this->m_game_types = static_cast< c_game_types* >( g_modules[ HASH_CT( "matchmaking.dll" ) ].find_interface( "VENGINE_GAMETYPES_VERSION002" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_localize =
	           static_cast< c_localize* >( g_modules[ HASH_CT( "localize.dll" ) ].find_interface( "Localize_001" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_key_values_system = reinterpret_cast< c_key_values_system*( __cdecl* )( ) >(
			   g_modules[ HASH_CT( "vstdlib.dll" ) ].find_export( "KeyValuesSystem" ) )( ) ) == nullptr )
		return false;
	else
		g_console.print(
			std::vformat( "found IKeyValuesSystem @ {:p}", std::make_format_args( reinterpret_cast< void* >( this->m_key_values_system ) ) )
				.c_str( ) );

	if ( ( this->m_global_vars_base =
	           **reinterpret_cast< c_global_vars_base*** >( g_virtual.get< unsigned int >( this->m_base_client, 11 ) + 0xa ) ) == nullptr )
		return false;
	else
		g_console.print(
			std::vformat( "found IGlobalVarsBase @ {:p}", std::make_format_args( reinterpret_cast< void* >( this->m_global_vars_base ) ) ).c_str( ) );

	if ( ( this->m_client_mode = **reinterpret_cast< void*** >( g_virtual.get< unsigned int >( this->m_base_client, 10 ) + 0x5 ) ) == nullptr )
		return false;
	else
		g_console.print(
			std::vformat( "found IClientModeShared @ {:p}", std::make_format_args( reinterpret_cast< void* >( this->m_client_mode ) ) ).c_str( ) );

	if ( ( this->m_weapon_system = *reinterpret_cast< c_weapon_system** >(
			   g_modules[ HASH_CT( "client.dll" ) ].find_pattern( ( "8B 35 ? ? ? ? FF 10 0F B7 C0" ) ) + 0x2 ) ) == nullptr )
		return false;
	else
		g_console.print(
			std::vformat( "found IWeaponSystem @ {:p}", std::make_format_args( reinterpret_cast< void* >( this->m_weapon_system ) ) ).c_str( ) );

	if ( ( this->m_client_state = **reinterpret_cast< c_client_state*** >(
			   g_modules[ HASH_CT( "engine.dll" ) ].find_pattern( ( "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) ) + 0x1 ) ) == nullptr )
		return false;
	else
		g_console.print(
			std::vformat( "found IClientState @ {:p}", std::make_format_args( reinterpret_cast< void* >( this->m_client_state ) ) ).c_str( ) );

	if ( ( this->m_input = *reinterpret_cast< c_input** >( g_modules[ HASH_CT( "client.dll" ) ].find_pattern( "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) +
	                                                       0x1 ) ) == nullptr )
		return false;
	else
		g_console.print( std::vformat( "found IInput @ {:p}", std::make_format_args( reinterpret_cast< void* >( this->m_input ) ) ).c_str( ) );

	if ( ( this->m_direct_device = **reinterpret_cast< IDirect3DDevice9*** >(
			   g_modules[ HASH_CT( "shaderapidx9.dll" ) ].find_pattern( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 0x1 ) ) == nullptr )
		return false;
	else
		g_console.print(
			std::vformat( "found IDirect3DDevice9 @ {:p}", std::make_format_args( reinterpret_cast< void* >( this->m_direct_device ) ) ).c_str( ) );

	return true;
}