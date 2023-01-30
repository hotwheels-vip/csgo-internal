#pragma once

#include "../dependencies/imgui/imgui.h"
#include "source_engine/classes/c_base_entity.h"
#include "source_engine/classes/c_user_cmd.h"
#include "source_engine/structs/matrix_t.h"

struct globals_t {
	c_base_entity* m_local      = nullptr;
	c_user_cmd* m_cmd           = nullptr;
	view_matrix_t m_view_matrix = { };
	ImVec2 m_display_size       = { };
	c_angle m_old_view_point    = { };
	bool m_console_being_drawn  = false;
	float m_last_tick_yaw       = { };

	struct {
		c_user_cmd* m_backup_cmd      = nullptr;
		c_base_entity* m_backup_local = nullptr;
	} backup;

	bool m_unloading = false;
};

inline globals_t globals = { };
