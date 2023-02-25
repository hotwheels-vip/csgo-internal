#pragma once

namespace n_edicts
{
	struct impl_t {
		void on_frame_stage_notify( int stage );
		void on_paint_traverse( );

		void reset( ); /* AAAAAA */
		void* get_precipitation_collideable( );

		void projectiles( );

		bool m_created = false;
		int m_timer    = -1;

	private:
		void dropped_weapons( );

		void precipitation( );
		void fog( );

		void disable_post_processing( );
	};
} // namespace n_edicts

inline n_edicts::impl_t g_edicts{ };
