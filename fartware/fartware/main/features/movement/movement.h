#pragma once

#include "../../convars/convars.h"
#include "../../includes.h"
#include "../../source_engine/classes/c_prediction.h"
#include "../../source_engine/enumerations/e_flags.h"
#include "../../source_engine/enumerations/e_move_types.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../prediction/prediction.h"

struct movement_t {
public:
	void on_create_move_pre( );
	void on_create_move_post( );

private:
};

inline movement_t movement = { };
