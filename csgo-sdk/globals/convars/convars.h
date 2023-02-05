#pragma once

class c_cconvar;

namespace n_convars
{
	struct impl_t {
		bool on_attach( );

		c_cconvar* operator[]( unsigned int hash );
	};
} // namespace n_convars

inline n_convars::impl_t g_convars{ };