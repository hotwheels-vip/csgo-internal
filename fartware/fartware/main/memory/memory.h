#pragma once

#include <d3d9.h>
#include <vector>

#include "../source_engine/classes/c_client_state.h"
#include "../source_engine/classes/c_globals.h"
#include "../source_engine/classes/c_input.h"
#include "../source_engine/classes/c_key_values_system.h"
#include "../source_engine/classes/c_prediction.h"
#include "../source_engine/classes/c_weapon_system.h"

enum e_module_names : int {
	client = 0,
	engine,
	vgui2,
	vguimatsurface,
	localize,
	server,
	panorama,
	materialsystem,
	vstdlib,
	shaderapidx9,
	inputsystem,
	steam_api,
	datacache,
	max
};

struct module_handle_info_t {
	const char* m_name = { };
	void* m_value      = { };

	std::uint8_t* find_pattern( const char* signature );
};

struct memory_t {
	bool on_attach( const std::vector< const char* >& modules );
	bool on_attach( );

	std::vector< module_handle_info_t > m_modules = { };

	c_input* m_input                         = nullptr;
	c_key_values_system* m_key_values_system = nullptr;
	c_client_state* m_client_state           = nullptr;
	c_weapon_system* m_weapon_system         = nullptr;
	c_globals* m_globals                     = nullptr;
	c_move_helper* m_move_helper             = nullptr;
	void* m_client_mode                      = nullptr;
	IDirect3DDevice9* m_device               = nullptr;
};

inline memory_t memory = { };