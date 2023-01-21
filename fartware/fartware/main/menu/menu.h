#pragma once

constexpr const char* base_material_names[ 2 ]    = { "flat", "textured" };
constexpr const char* overlay_material_names[ 1 ] = { "snow" };

struct menu_t {
	void on_end_scene( );

	bool m_opened            = true;
	float m_alpha            = 0.f;
	int m_selected_config    = 0;
	char m_config_file[ 32 ] = { };
};

inline menu_t menu = { };