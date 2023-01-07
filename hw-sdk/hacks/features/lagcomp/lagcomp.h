#pragma once

#include "../../../game/sdk/classes/c_cs_player.h"
#include "../../../globals/includes/includes.h"

namespace lagcomp
{
	class record
	{
	public:
		record( ) = default;

		bool valid            = false;
		float simulation_time = -1.f;

		math::vec3 abs_origin   = { };
		math::vec3 eye_position = { };

		sdk::c_cs_player* player = { };

		math::matrix_3x4 bone_matrix[ 128 ] = { };
	};

	struct impl {
	private:
		// You should kill yourself NOW!
		// [ 1-64 ] is the range of possible player indexes.
		std::array< int, 65 > heap_iterator;
		bool is_valid( record heap_record );

	public:
		float lerp_time( );

		std::array< record*, 65 > heap_records;

		void update( );

		void backtrack_player( record* heap_record );
		void backtrack_player( sdk::c_cs_player* player );
	};
} // namespace lagcomp

inline lagcomp::impl g_lagcomp;
