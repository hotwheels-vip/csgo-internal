#pragma once
#include <array>

#include "../../game/sdk/structs/matrix_t.h"

namespace n_lagcomp
{
	struct impl_t {
		struct record_t {
			int m_player = -1;

			float m_sim_time{ };
			int m_tick_count{ };

			bool m_valid{ };

			matrix3x4_t m_matrix[ 128 ]{ };
		};

		std::array< int, 65 > m_record_location{ };
		std::array< record_t*, 65 > m_records{ };

		void on_create_move_pre( );
		void on_create_move_post( );
	};
} // namespace n_lagcomp

inline n_lagcomp::impl_t g_lagcomp{ };