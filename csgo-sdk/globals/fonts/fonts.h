#pragma once
#include <string>
#include <vector>

namespace n_fonts
{
	struct impl_t {
		void on_attach( );

		std::vector< std::string > m_font_file_names{ };
	};
} // namespace n_fonts

inline n_fonts::impl_t g_fonts{ };