#pragma once
#include "../../game/sdk/structs/matrix_t.h"
#include <array>
#include <deque>
#include <optional>

class c_base_entity;
class c_net_channel;

namespace n_lagcomp
{
	struct impl_t {
		struct sequence_object_t {
			sequence_object_t( int in_reliable_state, int out_reliable_state, int sequence_nr, float current_time )
				: m_in_reliable_state( in_reliable_state ), m_out_reliable_state( out_reliable_state ), m_sequence_nr( sequence_nr ),
				  m_current_time( current_time ){ };

			int m_in_reliable_state{ };
			int m_out_reliable_state{ };
			int m_sequence_nr{ };
			float m_current_time{ };
		};

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

		void on_frame_stage_notify( );
		void on_create_move_post( );

		std::optional< n_lagcomp::impl_t::record_t > oldest_record( const int ent_index );
		bool generate_lerped_lag_matrix( const int ent_index, matrix3x4_t* out );

		void backtrack_player( record_t* heap_record );
		void backtrack_player( c_base_entity* player );

		void update_incoming_sequences( c_net_channel* net_channel );
		void clear_incoming_sequences( );
		void add_latency_to_net_channel( c_net_channel* net_channel, float latency );
		void on_create_move_update( c_net_channel* net_channel );

	private:
		std::deque< sequence_object_t > m_sequences = { };
		int m_real_incoming_sequence                = 0;
		int m_last_incoming_sequence                = 0;
	};
} // namespace n_lagcomp

inline n_lagcomp::impl_t g_lagcomp{ };
