#pragma once
#include "key_bind_t.h"

#include <Windows.h>

#include <array>

enum class e_key_state : int {
	none = 0,
	down,
	up,
	released
};

namespace n_input
{
	struct impl_t {
		HWND m_window          = nullptr;
		WNDPROC m_old_wnd_proc = nullptr;

		std::array< e_key_state, 256U > m_key_state = { };

		bool on_attach( );
		void on_release( );
		bool on_wndproc( unsigned int msg, unsigned int wide_param, long long_param );

		bool is_key_down( const std::uint32_t button_code )
		{
			return this->m_key_state[ button_code ] == e_key_state::down;
		}

		bool is_key_released( const std::uint32_t button_code )
		{
			if ( this->m_key_state[ button_code ] == e_key_state::released ) {
				this->m_key_state[ button_code ] = e_key_state::up;
				return true;
			}

			return false;
		}

		bool check_input( key_bind_t* key_data )
		{
			switch ( key_data->m_key_style ) {
			case 0: {
				return true;
				break;
			}
			case 1: {
				return this->is_key_down( key_data->m_key );
				break;
			}
			case 2: {
				static bool toggled = false;

				if ( this->is_key_released( key_data->m_key ) )
					toggled = !toggled;

				return toggled;
				break;
			}
			}

			return false;
		}
	};
} // namespace n_input

inline n_input::impl_t g_input{ };