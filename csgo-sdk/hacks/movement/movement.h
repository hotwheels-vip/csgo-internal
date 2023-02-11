#pragma once

class c_user_cmd;
class c_angle;

namespace n_movement
{

	struct impl_t {
		void on_create_move_pre( );
		void on_create_move_post( int pre_prediction_flags, const c_angle& old_view_point );

	private:
		void bunny_hop( );

		void edge_jump( int pre_prediction_flags );
		void long_jump( );
		void mini_jump( int pre_prediction_flags );
		void jump_bug( int pre_prediction_flags );

		void rotate_movement( c_user_cmd* cmd, const c_angle& angle );
		void auto_align( c_user_cmd* cmd );

		void movement_fix( const c_angle& old_view_point );
	};
} // namespace n_movement

inline n_movement::impl_t g_movement{ };