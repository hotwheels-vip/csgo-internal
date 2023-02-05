#pragma once

struct module_t {
public:
	module_t( ) = default;
	module_t( void* value )
	{
		this->m_value = value;
	}

	unsigned char* find_pattern( const char* signature );

	void* find_interface( const char* interface_name );

	void* find_export( const char* export_name );

	void* get_value( )
	{
		return this->m_value;
	}

private:
	void* m_value{ };
};

namespace n_modules
{
	struct impl_t {
		bool on_attach( );

		module_t operator[]( unsigned int hash );
	};
} // namespace n_modules

inline n_modules::impl_t g_modules{ };