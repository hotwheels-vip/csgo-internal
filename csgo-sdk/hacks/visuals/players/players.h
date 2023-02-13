#pragma once

namespace n_players
{
	struct impl_t {
		void on_paint_traverse( );
	private:
		void players( );
	};
} // namespace n_players

inline n_players::impl_t g_players{ };