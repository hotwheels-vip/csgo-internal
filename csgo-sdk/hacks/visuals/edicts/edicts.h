#pragma once

namespace n_edicts
{
	struct impl_t {
		void on_frame_stage_notify( );
		void reset( ); /* AAAAAA */
		void* get_precipitation_collideable( );

		bool m_created = false;
		int m_timer    = -1;
	};
} // namespace n_edicts

inline n_edicts::impl_t g_edicts{ };