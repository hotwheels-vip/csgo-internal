#pragma once
#include "../../../source_engine/enumerations/e_client_frame_stage.h"

struct world_t {
	void on_frame_stage_notify( e_client_frame_stage );
};

inline world_t world = { };
