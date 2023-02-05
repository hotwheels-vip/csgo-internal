#pragma once

class c_base_client_dll;
class c_engine_client;
class c_client_entity_list;
class c_input_system;
class c_material_system;
class c_engine_trace;
class c_debug_overlay;
class c_convar;
class c_input;

class c_key_values_system;
class c_global_vars_base;
class c_weapon_system;
class c_client_state;

struct IDirect3DDevice9;

namespace n_interfaces
{
	struct impl_t {
		bool on_attach( );

		c_base_client_dll* m_base_client           = nullptr;
		c_engine_client* m_engine_client           = nullptr;
		c_client_entity_list* m_client_entity_list = nullptr;
		c_input_system* m_input_system             = nullptr;
		c_material_system* m_material_system       = nullptr;
		c_engine_trace* m_engine_trace             = nullptr;
		c_debug_overlay* m_debug_overlay           = nullptr;
		c_convar* m_convar                         = nullptr;

		c_key_values_system* m_key_values_system = nullptr;
		c_global_vars_base* m_global_vars_base   = nullptr;
		void* m_client_mode                      = nullptr;
		c_weapon_system* m_weapon_system         = nullptr;
		c_client_state* m_client_state           = nullptr;
		c_input* m_input                         = nullptr;
		IDirect3DDevice9* m_direct_device        = nullptr;
	};
} // namespace n_interfaces

inline n_interfaces::impl_t g_interfaces{ };