#pragma once

#include "../../includes.h"

#include "../../convars/convars.h"

struct misc_t {
	struct practice_t {
		void render( );
		void think( );

		c_vector pos{ };
		c_angle ang{ };
	} practice;
};

inline misc_t misc = { };
