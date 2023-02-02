#pragma once
#include "../../source_engine/structs/matrix_t.h"
#include <array>
#include <iostream>
class c_base_entity;

struct lagcomp_t {
	struct record {
		bool valid            = false;
		float simulation_time = -1.f;

		c_vector abs_origin   = { };
		c_vector eye_position = { };

		c_base_entity* player = { };

		matrix3x4_t bone_matrix[ 128 ] = { };
	};

private:
	std::array< int, 65 > heap_iterator;

public:
	std::array< record*, 65 > heap_records;

	void update( );

	// utils

	float lerp_time( );

	int max_ticks( );

	bool is_valid( record rec );

	void backtrack_player( record* heap_record );
	void backtrack_player( c_base_entity* player );
};

inline lagcomp_t lagcomp = { };
