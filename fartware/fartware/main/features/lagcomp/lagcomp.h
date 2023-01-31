#pragma once
#include "../../source_engine/structs/matrix_t.h"
#include <array>
#include <iostream>

struct lagcomp_t {
	struct record {
		// For record validation purposes.
		int player_index = -1;

		float sim_time{ };
		int tick_count{ };

		bool valid{ };

		matrix3x4_t matrix[ 128 ]{ };
	};

	std::array< int, 65 > record_location{ };
	std::array< record*, 65 > records{ };

	void update( );

	void run( record rec );

	// utils

	float lerp_time( );

	bool is_valid( record rec );
};

inline lagcomp_t lagcomp = { };
