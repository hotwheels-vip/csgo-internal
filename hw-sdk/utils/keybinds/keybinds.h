#pragma once

#include "../../dependencies/lazy_importer/lazy_importer.h"
#include "../math/types/vector.h"
#include <Windows.h>
#include <deque>
#include <functional>

namespace input
{
	enum key_state_t : int {
		KEY_NONE,
		KEY_UP,
		KEY_DOWN,
		KEY_RELEASED
	};

	constexpr static inline std::uint64_t MAX_RELEASED_TIME = 100;

	struct keybind {
		bool awaiting_input{ };
		std::function< void( std::uint8_t, bool ) > poll_callback;

		std::uint8_t virtual_key;
		std::function< void( bool pressed ) > callback;
	};

	struct impl {
	private:
		struct key_info_t {
			input::key_state_t state;
			std::uint64_t time;

			constexpr key_info_t( ) : state{ input::key_state_t::KEY_NONE }, time{ 0 } { }

			constexpr key_info_t( input::key_state_t state, std::uint64_t time ) : state{ state }, time{ time } { }
		};

	public:
		std::array< key_info_t, 256U > key_states = { };
		std::deque< keybind > key_binds           = { };

		struct {
			math::vec2< int > pos;
			int scroll_amt;

			bool in_params( int x, int y, int w, int h )
			{
				return pos.x >= x && pos.y >= y && pos.x <= ( x + w ) && pos.y <= ( y + h );
			};
			inline bool in_params( const math::vec2< int >& m_pos, const math::vec2< int >& m_size )
			{
				return in_params( m_pos.x, m_pos.y, m_size.x, m_size.y );
			}
		} mouse;

		void think( UINT msg, WPARAM wparam, LPARAM lparam );

		bool key_state( input::key_state_t state, int key_id )
		{
			auto& key = key_states[ key_id ];

			static auto lazy_get_tickcount = LI_FN( GetTickCount64 ).get( );

			std::uint64_t time = lazy_get_tickcount( );

			if ( state == key_state_t::KEY_RELEASED ) {
				if ( key.state == state )
					if ( time - key.time <= input::MAX_RELEASED_TIME )
						return key.state = key_state_t::KEY_UP;
					else
						key.state = key_state_t::KEY_UP;

				return false;
			} else
				return key.state == state;
		}

		void add_keybind( std::uint8_t virtual_key, std::function< void( bool ) > callback );
		void remove_keybind( std::uint8_t virtual_key );

		void poll_keybind( std::function< void( std::uint8_t, bool ) > callback, std::function< void( bool ) > callback_fail = nullptr );

		char* key_to_char( std::uint8_t virtual_key );
	};
} // namespace input

inline input::impl g_input;
