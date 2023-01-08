#pragma once

#include "source_engine/classes/c_base_entity.h"
#include "source_engine/classes/c_user_cmd.h"
#include "source_engine/structs/matrix_t.h"
#include "../dependencies/imgui/imgui.h"

struct globals_t {
	c_base_entity* m_local = nullptr;
	c_user_cmd* m_cmd      = nullptr;
	view_matrix_t m_view_matrix = { };
	ImVec2 m_display_size       = { };
};

inline globals_t globals = { };