#pragma once
#include <deque>

struct indicators_t {
	struct velocity_data {
		int m_velocity{ };
		bool m_edgebugged{ };
		bool m_jumpbugged{ };
	};
	struct detection_data {
		void reset( );
		bool m_jumpbugged{ };
		bool m_edgebugged{ };
	} detection;

	void on_paint_traverse( );

	void on_create_move_pre( );

	void on_create_move_post( );
};

inline std::deque< indicators_t::velocity_data > velocity_history{ };

inline indicators_t indicators = { };
