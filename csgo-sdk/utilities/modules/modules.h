#pragma once

struct module_handle_t {
	module_handle_t( ){ };
	module_handle_t( void* value )
	{
		this->m_value = value;
	}

	void* get_value() {
		return this->m_value;
	}

	const char* get_name() {
		return this->m_name;
	}

	unsigned char* find( const char* signature );

private:
	void* m_value{ };
	const char* m_name{ };
};

namespace n_modules
{
	struct impl_t {
		bool on_attach( );

		module_handle_t operator[]( unsigned int hash );
	};
} // namespace n_modules

inline n_modules::impl_t g_modules{ };