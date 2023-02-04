#pragma once

class c_base_client_dll;
class c_engine_client;

namespace n_interfaces
{
	struct impl_t {
		bool on_attach( );

		c_base_client_dll* m_base_client = nullptr;
		c_engine_client* m_engine_client = nullptr;
	};
} // namespace n_interfaces

inline n_interfaces::impl_t g_interfaces{ };