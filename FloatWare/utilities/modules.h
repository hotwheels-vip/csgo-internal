#pragma once
#include <vector>

struct module_info_t {
public:
	const char* m_name = nullptr;
	void* m_value      = nullptr;

	unsigned char* find_pattern( const char* signature );
};

enum e_module_names : int {
	client = 0,
	engine
};

namespace modules
{
	struct impl {
	public:
		bool on_attach( const std::vector< const char* >& modules );

		module_info_t get(e_module_names module_index) {
			return this->m_modules[ module_index ];
		}

	private:
		std::vector< module_info_t > m_modules = { };
	};
} // namespace modules

inline modules::impl g_modules;