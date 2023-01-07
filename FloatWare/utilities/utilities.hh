#pragma once
#include <string_view>

namespace utilities
{
	struct impl {
	public:
		void* get_module_handle( const std::string_view module_name );
	};
} // namespace utilities

inline utilities::impl g_utilities;