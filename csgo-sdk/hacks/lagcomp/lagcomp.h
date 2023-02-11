#pragma once
#include <array>

namespace n_lagcomp
{
	struct impl_t {
		struct record_t {
		};

		std::array< int, 65 > m_record_location{ };
		std::array< record_t*, 65 > m_records{ };
	};
} // namespace n_lagcomp

inline n_lagcomp::impl_t g_lagcomp{ };