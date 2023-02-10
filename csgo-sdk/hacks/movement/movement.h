#pragma once

namespace n_movement
{

	struct impl_t {
		void on_create_move_pre(  );
		void on_create_move_post( int pre_prediction_flags );

	private:
		void bunny_hop( );

		void edge_jump( int pre_prediction_flags );
		void long_jump( );
		void mini_jump( int pre_prediction_flags );
		void jump_bug( int pre_prediction_flags );

		void auto_align( );
	};
} // namespace n_movement

inline n_movement::impl_t g_movement{ };