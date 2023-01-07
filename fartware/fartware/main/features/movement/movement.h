#pragma once

#include "../../source_engine/classes/c_prediction.h"

struct movement_t {
	void on_create_move_pre( );
	void on_create_move_post( );

private:
};

inline movement_t movement = { };