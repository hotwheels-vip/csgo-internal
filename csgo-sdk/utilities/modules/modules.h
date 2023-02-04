#pragma once

namespace n_modules
{
	struct impl_t {
		bool on_attach( );

		void* operator[]( unsigned int hash );

	private:
	};
} // namespace n_modules

inline n_modules::impl_t g_modules{ };