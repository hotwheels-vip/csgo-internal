#pragma once

constexpr const char* base_material_names[ 2 ]    = { "flat", "textured" };
constexpr const char* overlay_material_names[ 1 ] = { "snow" };

namespace n_menu
{
	struct impl_t {
		bool m_opened = false;

		int m_selected_config    = 0;
		char m_config_file[ 32 ] = { };
	};
} // namespace n_menu

inline n_menu::impl_t g_menu{ };