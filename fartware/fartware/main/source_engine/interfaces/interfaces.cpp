#include "../../../dependencies/steam/steam_api.h"
#include "../../includes.h"

template< typename T >
T* interfaces_t::find( const void* module_handle, const std::string_view interface_name )
{
	const auto get_register_list = [ & ]( ) -> c_interface_register* {
		const void* create_interface_export = utilities.get_export_address( module_handle, ( "CreateInterface" ) );

		if ( !create_interface_export )
			throw std::runtime_error( ( "failed get createinterface address" ) );

		const std::uintptr_t create_interface_relative = reinterpret_cast< std::uintptr_t >( create_interface_export ) + 0x5;
		const std::uintptr_t create_interface = create_interface_relative + 4U + *reinterpret_cast< std::int32_t* >( create_interface_relative );
		return **reinterpret_cast< c_interface_register*** >( create_interface + 0x6 );
	};

	for ( auto register_data = get_register_list( ); register_data; register_data = register_data->m_next ) {
		if ( ( std::string_view( register_data->m_name ).compare( 0U, interface_name.length( ), interface_name ) == 0 &&
		       std::atoi( register_data->m_name + interface_name.length( ) ) > 0 ) ||
		     interface_name.compare( register_data->m_name ) == 0 ) {
			auto interface_address = register_data->m_create_fn( );

			console.print( ( "found {:s} interface @ {:p}" ), register_data->m_name, interface_address );

			return static_cast< T* >( interface_address );
		}
	}

	console.print( ( "failed to capture interface {:s}" ), interface_name );

	return nullptr;
}

bool interfaces_t::on_attach( )
{
	if ( ( this->m_client = this->find< c_client >( memory.m_modules[ e_module_names::client ].m_value, ( "VClient" ) ) ) == nullptr )
		return false;

	if ( ( this->m_engine = this->find< c_engine >( memory.m_modules[ e_module_names::engine ].m_value, ( "VEngineClient" ) ) ) == nullptr )
		return false;

	if ( ( this->m_engine_sound = this->find< void >( memory.m_modules[ e_module_names::engine ].m_value, ( "IEngineSoundClient" ) ) ) == nullptr )
		return false;

	if ( ( this->m_surface = this->find< c_surface >( memory.m_modules[ e_module_names::vguimatsurface ].m_value, ( "VGUI_Surface" ) ) ) == nullptr )
		return false;

	if ( ( this->m_input_system =
	           this->find< c_input_system >( memory.m_modules[ e_module_names::inputsystem ].m_value, ( "InputSystemVersion" ) ) ) == nullptr )
		return false;

	if ( ( this->m_client_entity_list =
	           this->find< c_client_entity_list >( memory.m_modules[ e_module_names::client ].m_value, ( "VClientEntityList" ) ) ) == nullptr )
		return false;

	if ( ( this->m_localize = this->find< c_localize >( memory.m_modules[ e_module_names::localize ].m_value, ( "Localize_" ) ) ) == nullptr )
		return false;

	if ( ( this->m_panel = this->find< c_panel >( memory.m_modules[ e_module_names::vgui2 ].m_value, ( "VGUI_Panel" ) ) ) == nullptr )
		return false;

	if ( ( this->m_convar = this->find< c_convar >( memory.m_modules[ e_module_names::vstdlib ].m_value, ( "VEngineCvar" ) ) ) == nullptr )
		return false;

	if ( ( this->m_game_movement = this->find< c_game_movement >( memory.m_modules[ e_module_names::client ].m_value, ( "GameMovement" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_prediction = this->find< c_prediction >( memory.m_modules[ e_module_names::client ].m_value, ( "VClientPrediction" ) ) ) ==
	     nullptr )
		return false;

	if ( ( this->m_mdl_cache = this->find< c_mdl_cache >( memory.m_modules[ e_module_names::datacache ].m_value, ( "MDLCache" ) ) ) == nullptr )
		return false;

	if ( ( this->m_engine_trace =
	           this->find< c_engine_trace_client >( memory.m_modules[ e_module_names::engine ].m_value, ( "EngineTraceClient" ) ) ) == nullptr )
		return false;

	if ( ( this->m_model_render =
	           this->find< c_model_render >( memory.m_modules[ e_module_names::engine ].m_value, ( "VEngineModel" ) ) ) == nullptr )
		return false;

	/* TODO ~ float ~ snake_caseify this */
	SteamClient = this->m_engine->steam_api_context( )->m_steam_client;
	if ( !SteamClient )
		return false;

	SteamUser = this->m_engine->steam_api_context( )->m_steam_user;
	if ( !SteamUser )
		return false;

	SteamFriends = this->m_engine->steam_api_context( )->m_steam_friends;
	if ( !SteamFriends )
		return false;

	SteamUtils = this->m_engine->steam_api_context( )->m_steam_utils;
	if ( !SteamUtils )
		return false;

	const int steam_user = reinterpret_cast< std::add_pointer_t< int( ) > >(
		utilities.get_export_address( memory.m_modules[ e_module_names::steam_api ].m_value, ( "SteamAPI_GetHSteamUser" ) ) )( );
	const int steam_pipe = reinterpret_cast< std::add_pointer_t< int( ) > >(
		utilities.get_export_address( memory.m_modules[ e_module_names::steam_api ].m_value, ( "SteamAPI_GetHSteamPipe" ) ) )( );

	SteamGameCoordinator =
		static_cast< ISteamGameCoordinator* >( SteamClient->GetISteamGenericInterface( steam_user, steam_pipe, ( "SteamGameCoordinator001" ) ) );
	if ( !SteamGameCoordinator )
		return false;

	return true;
}
