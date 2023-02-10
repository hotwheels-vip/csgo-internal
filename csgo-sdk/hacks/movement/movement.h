#pragma once

namespace n_movement
{

struct impl_t {
		void on_create_move_post( );

private:
		void auto_align( );
	
};
} // namespace n_movement

inline n_movement::impl_t g_movement{ };