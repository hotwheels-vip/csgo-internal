#pragma once

#include "../../includes.h"

#include "../../convars/convars.h"

struct misc_t {
	struct practice_t {
		c_angle saved_angles;
		c_vector saved_position;
	} practice;

	void on_end_scene( );
	void on_create_move_pre( );
};

inline misc_t misc = { };
