#pragma once
#pragma once

namespace n_animations
{
	struct impl_t {
		void on_frame_stage_notify( int stage );
	};
} // namespace n_animations

inline n_animations::impl_t g_animations{ };
