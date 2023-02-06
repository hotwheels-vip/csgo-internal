#pragma once
#include <unordered_map>

struct module_t {
public:
	module_t( ) = default;
	module_t( void* value, const char* name )
	{
		this->m_value = value;
		this->m_name  = name;
	}

	unsigned char* find_pattern( const char* signature );

	void* find_interface( const char* interface_name );

	void* find_export( const char* export_name );

	void* get_value( )
	{
		return this->m_value;
	}

	const char* get_name( )
	{
		return this->m_name;
	}

private:
	void* m_value{ };
	const char* m_name{ };
};

namespace n_modules
{
	struct impl_t {
		bool on_attach( );

		module_t operator[]( unsigned int hash );

		std::unordered_map< unsigned int, module_t > m_modules = { };
	};
} // namespace n_modules

inline n_modules::impl_t g_modules{ };