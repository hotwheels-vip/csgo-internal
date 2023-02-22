#pragma once
#include <array>

#include "../../game/sdk/structs/matrix_t.h"

class c_base_entity;

namespace n_lagcomp
{
	struct impl_t {
		struct record_t {
			int m_player = -1;

			float m_sim_time{ };
			int m_tick_count{ };

			bool m_valid{ };

			c_vector m_vec_origin{ };

			matrix3x4_t m_matrix[ 128 ]{ };
		};

		std::array< int, 65 > m_record_location{ };
		std::array< record_t*, 65 > m_records{ };

		float lerp_time( );
		bool is_valid( n_lagcomp::impl_t::record_t rec );

		void on_create_move_pre( );
		void on_create_move_post( );

		void backtrack_player( record_t* heap_record );
		void backtrack_player( c_base_entity* player );
	};
} // namespace n_lagcomp

inline n_lagcomp::impl_t g_lagcomp{ };
