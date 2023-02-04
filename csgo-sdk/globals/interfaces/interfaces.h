#pragma once

class c_base_client_dll;
class c_engine_client;
class c_client_entity_list;
class c_input_system;

class c_key_values_system;

namespace n_interfaces
{
	struct impl_t {
		bool on_attach( );

		c_base_client_dll* m_base_client           = nullptr;
		c_engine_client* m_engine_client           = nullptr;
		c_client_entity_list* m_client_entity_list = nullptr;
		c_input_system* m_input_system             = nullptr;

		c_key_values_system* m_key_values_system = nullptr;
	};
} // namespace n_interfaces

inline n_interfaces::impl_t g_interfaces{ };