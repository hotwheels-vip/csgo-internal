#pragma once
#include "../../dependencies/mocking_bird/mocking_bird.h"
#include "../../globals/includes/includes.h"
#include "../helpers/function_enforce/function_enforce.h"
#include "address/address.h"

namespace modules
{
	struct interface_node {
	public:
		void* ( *get )( );
		const char* name;
		interface_node* next;
	};

	class impl
	{
	public:
		impl( ) = default;
		explicit impl( const char* mod_name )
		{
			module_name   = mod_name;
			module_handle = get_module_handle_safe( mod_name );
		};

		address pattern_scan( const char* sig );

		template< class T >
		T find_interface( const char* interface_name );

		constexpr HMODULE get_module_handle_safe( const char* name );

		// public for pointer->module
		HMODULE module_handle{ };

	private:
		const char* module_name{ };

		std::uintptr_t find_pattern( std::uint8_t* region_start, std::uintptr_t region_size, const char* pattern );

		std::vector< int > pattern_to_bytes( const char* pattern );
	};
} // namespace modules

// global modules

inline modules::impl g_client_dll                = modules::impl( CLIENT_DLL );
inline modules::impl g_engine_dll                = modules::impl( ENGINE_DLL );
inline modules::impl g_localize_dll              = modules::impl( LOCALIZE_DLL );
inline modules::impl g_shaderapidx9_dll          = modules::impl( SHADERAPIDX9_DLL );
inline modules::impl g_vstdlib_dll               = modules::impl( VSTDLIB_DLL );
inline modules::impl g_server_dll                = modules::impl( SERVER_DLL );
inline modules::impl g_studio_render_dll         = modules::impl( STUDIORENDER_DLL );
inline modules::impl g_material_system_dll       = modules::impl( MATERIALSYSTEM_DLL );
inline modules::impl g_vgui_dll                  = modules::impl( VGUI_DLL );
inline modules::impl g_vgui2_dll                 = modules::impl( VGUI2_DLL );
inline modules::impl g_game_overlay_renderer_dll = modules::impl( GAMEOVERLAYRENDERER_DLL );
inline modules::impl g_physics_dll               = modules::impl( PHYSICS_DLL );
inline modules::impl g_tier0_dll                 = modules::impl( TIER0_DLL );
inline modules::impl g_input_system_dll          = modules::impl( INPUTSYSTEM_DLL );
inline modules::impl g_data_cache_dll            = modules::impl( DATACACHE_DLL );
inline modules::impl g_steam_api_dll             = modules::impl( STEAM_API_DLL );
inline modules::impl g_matchmaking_dll           = modules::impl( MATCHMAKING_DLL );
inline modules::impl g_file_system_stdio_dll     = modules::impl( FILESYSTEM_DLL );

#include "modules.inl"
