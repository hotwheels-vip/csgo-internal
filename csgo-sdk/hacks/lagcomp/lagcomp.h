#pragma once
#include "../../game/sdk/structs/matrix_t.h"
#include <array>
#include <optional>

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

			// used for chams.
			matrix3x4_t m_matrix_interpolated[ 128 ]{ };
		};

		std::array< int, 65 > m_record_location{ };
		std::array< record_t*, 65 > m_records{ };

		float lerp_time( );
		bool is_valid( n_lagcomp::impl_t::record_t rec );

		void on_frame_stage_notify( );
		void on_create_move_post( );

		std::optional< n_lagcomp::impl_t::record_t > oldest_record( const int ent_index );
		bool generate_lerped_lag_matrix( const int ent_index, matrix3x4_t* out );

		void backtrack_player( record_t* heap_record );
		void backtrack_player( c_base_entity* player );
	};
} // namespace n_lagcomp

inline n_lagcomp::impl_t g_lagcomp{ };
