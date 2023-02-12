#pragma once

namespace n_misc
{
	struct impl_t {
		void on_create_move_pre( );
	private:
		void disable_post_processing( );
		void remove_panorama_blur( );
	};
} // namespace n_misc

inline n_misc::impl_t g_misc{ };