#pragma once

#include "../../../dependencies/steam/isteamclient.h"
#include "../../../dependencies/steam/isteamgamecoordinator.h"
#include "../classes/c_client.h"
#include "../classes/c_client_entity_list.h"
#include "../classes/c_convar.h"
#include "../classes/c_engine.h"
#include "../classes/c_engine_trace.h"
#include "../classes/c_game_event_manager.h"
#include "../classes/c_input_system.h"
#include "../classes/c_localize.h"
#include "../classes/c_material_system.h"
#include "../classes/c_mdl_cache.h"
#include "../classes/c_panel.h"
#include "../classes/c_prediction.h"
#include "../classes/c_surface.h"

#include <string_view>

using instantiate_interface_fn = void*( __cdecl* )( );

class c_interface_register
{
public:
	instantiate_interface_fn m_create_fn = { };
	const char* m_name                   = { };
	c_interface_register* m_next         = { };
};

struct interfaces_t {
	bool on_attach( );

	template< typename T >
	T* find( const void* module_handle, const std::string_view interface_name );

	c_client* m_client                         = nullptr;
	c_engine* m_engine                         = nullptr;
	c_surface* m_surface                       = nullptr;
	c_input_system* m_input_system             = nullptr;
	c_client_entity_list* m_client_entity_list = nullptr;
	c_localize* m_localize                     = nullptr;
	c_panel* m_panel                           = nullptr;
	c_convar* m_convar                         = nullptr;
	c_prediction* m_prediction                 = nullptr;
	c_game_movement* m_game_movement           = nullptr;
	void* m_engine_sound                       = nullptr;
	c_mdl_cache* m_mdl_cache                   = nullptr;
	c_engine_trace* m_engine_trace             = nullptr;
	c_model_render* m_model_render             = nullptr;
	c_material_system* m_material_system       = nullptr;
	c_game_event_manager2* m_game_event        = nullptr;
};

inline interfaces_t interfaces = { };
