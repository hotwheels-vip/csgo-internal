#include "keybinds.h"

#include "../../globals/macros/macros.h"
#include <windowsx.h>

void input::impl::think( UINT msg, WPARAM wparam, LPARAM lparam )
{
	int key_id            = 0;
	key_state_t key_state = key_state_t::KEY_UP;

	mouse.scroll_amt = 0;

	switch ( msg ) {
	case WM_KEYDOWN:
		key_id    = wparam;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_KEYUP:
		key_id    = wparam;
		key_state = key_state_t::KEY_UP;
		break;
	case WM_LBUTTONDOWN:
		key_id    = VK_LBUTTON;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_LBUTTONUP:
		key_id    = VK_LBUTTON;
		key_state = key_state_t::KEY_UP;
		break;
	case WM_LBUTTONDBLCLK:
		key_id    = VK_LBUTTON;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_RBUTTONDOWN:
		key_id    = VK_RBUTTON;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_RBUTTONUP:
		key_id    = VK_RBUTTON;
		key_state = key_state_t::KEY_UP;
		break;
	case WM_RBUTTONDBLCLK:
		key_id    = VK_RBUTTON;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_MBUTTONDOWN:
		key_id    = VK_MBUTTON;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_MBUTTONUP:
		key_id    = VK_MBUTTON;
		key_state = key_state_t::KEY_UP;
		break;
	case WM_MBUTTONDBLCLK:
		key_id    = VK_MBUTTON;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_XBUTTONDOWN:
		key_id    = GET_XBUTTON_WPARAM( wparam ) == XBUTTON1 ? XBUTTON1 : XBUTTON2;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_XBUTTONUP:
		key_id    = GET_XBUTTON_WPARAM( wparam ) == XBUTTON1 ? XBUTTON1 : XBUTTON2;
		key_state = key_state_t::KEY_UP;
		break;
	case WM_XBUTTONDBLCLK:
		key_id    = GET_XBUTTON_WPARAM( wparam ) == XBUTTON1 ? XBUTTON1 : XBUTTON2;
		key_state = key_state_t::KEY_DOWN;
		break;
	case WM_MOUSEMOVE:
		mouse.pos = math::vec2< int >( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) );
		break;
	case WM_MOUSEWHEEL:
		mouse.scroll_amt = -( GET_WHEEL_DELTA_WPARAM( wparam ) / ( WHEEL_DELTA / 4 ) );
		break;
	case WM_KILLFOCUS:
	case WM_SETFOCUS:
		for ( auto& key : key_states )
			key.state = key_state_t::KEY_UP;
		break;
	default:
		break;
	}

	static auto lazy_get_tickcount = LI_FN( GetTickCount64 ).get( );

	std::uint64_t time = lazy_get_tickcount( );

	if ( key_id ) {
		if ( key_state == key_state_t::KEY_UP && key_states[ key_id ].state == key_state_t::KEY_DOWN )
			key_states[ key_id ] = { key_state_t::KEY_RELEASED, time };
		else
			key_states[ key_id ] = { key_state, time };

		for ( auto iterator = key_binds.begin( ); iterator != key_binds.end( ); ) {
			if ( iterator->awaiting_input ) {
				if ( key_id != VK_ESCAPE && key_id != VK_LBUTTON && key_id != VK_RBUTTON )
					iterator->poll_callback( key_id, key_state == key_state_t::KEY_UP ? false : key_state == key_state_t::KEY_DOWN );
				else
					iterator->callback( key_state == key_state_t::KEY_UP ? false : key_state == key_state_t::KEY_DOWN );
			}

			if ( iterator->virtual_key == key_id )
				iterator->callback( key_state == key_state_t::KEY_UP ? false : key_state == key_state_t::KEY_DOWN );

			iterator++;
		}
	}
}

void input::impl::add_keybind( std::uint8_t virtual_key, std::function< void( bool ) > callback )
{
	key_binds.push_back( { false, nullptr, virtual_key, callback } );
}

void input::impl::remove_keybind( std::uint8_t virtual_key )
{
	for ( auto iterator = key_binds.begin( ); iterator != key_binds.end( ); ) {
		if ( iterator->virtual_key == virtual_key )
			iterator = key_binds.erase( iterator );
		else
			iterator++;
	}
}

void input::impl::poll_keybind( std::function< void( std::uint8_t, bool ) > callback, std::function< void( bool ) > callback_fail )
{
	key_binds.push_back( { true, callback, 0x0, callback_fail } );
}

char* input::impl::key_to_char( std::uint8_t virtual_key )
{
	const char* key_names[] = { _( "Unknown" ),
		                        _( "LBUTTON" ),
		                        _( "RBUTTON" ),
		                        _( "CANCEL" ),
		                        _( "MBUTTON" ),
		                        _( "XBUTTON1" ),
		                        _( "XBUTTON2" ),
		                        _( "Unknown" ),
		                        _( "BACK" ),
		                        _( "TAB" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "CLEAR" ),
		                        _( "RETURN" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "SHIFT" ),
		                        _( "CONTROL" ),
		                        _( "MENU" ),
		                        _( "PAUSE" ),
		                        _( "CAPITAL" ),
		                        _( "KANA" ),
		                        _( "Unknown" ),
		                        _( "JUNJA" ),
		                        _( "FINAL" ),
		                        _( "KANJI" ),
		                        _( "Unknown" ),
		                        _( "ESCAPE" ),
		                        _( "CONVERT" ),
		                        _( "NONCONVERT" ),
		                        _( "ACCEPT" ),
		                        _( "MODECHANGE" ),
		                        _( "SPACE" ),
		                        _( "PRIOR" ),
		                        _( "NEXT" ),
		                        _( "END" ),
		                        _( "HOME" ),
		                        _( "LEFT" ),
		                        _( "UP" ),
		                        _( "RIGHT" ),
		                        _( "DOWN" ),
		                        _( "SELECT" ),
		                        _( "PRINT" ),
		                        _( "EXECUTE" ),
		                        _( "SNAPSHOT" ),
		                        _( "INSERT" ),
		                        _( "DELETE" ),
		                        _( "HELP" ),
		                        _( "0" ),
		                        _( "1" ),
		                        _( "2" ),
		                        _( "3" ),
		                        _( "4" ),
		                        _( "5" ),
		                        _( "6" ),
		                        _( "7" ),
		                        _( "8" ),
		                        _( "9" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "A" ),
		                        _( "B" ),
		                        _( "C" ),
		                        _( "D" ),
		                        _( "E" ),
		                        _( "F" ),
		                        _( "G" ),
		                        _( "H" ),
		                        _( "I" ),
		                        _( "J" ),
		                        _( "K" ),
		                        _( "L" ),
		                        _( "M" ),
		                        _( "N" ),
		                        _( "O" ),
		                        _( "P" ),
		                        _( "Q" ),
		                        _( "R" ),
		                        _( "S" ),
		                        _( "T" ),
		                        _( "U" ),
		                        _( "V" ),
		                        _( "W" ),
		                        _( "X" ),
		                        _( "Y" ),
		                        _( "Z" ),
		                        _( "LWIN" ),
		                        _( "RWIN" ),
		                        _( "APPS" ),
		                        _( "Unknown" ),
		                        _( "SLEEP" ),
		                        _( "NUMPAD0" ),
		                        _( "NUMPAD1" ),
		                        _( "NUMPAD2" ),
		                        _( "NUMPAD3" ),
		                        _( "NUMPAD4" ),
		                        _( "NUMPAD5" ),
		                        _( "NUMPAD6" ),
		                        _( "NUMPAD7" ),
		                        _( "NUMPAD8" ),
		                        _( "NUMPAD9" ),
		                        _( "MULTIPLY" ),
		                        _( "ADD" ),
		                        _( "SEPARATOR" ),
		                        _( "SUBTRACT" ),
		                        _( "DECIMAL" ),
		                        _( "DIVIDE" ),
		                        _( "F1" ),
		                        _( "F2" ),
		                        _( "F3" ),
		                        _( "F4" ),
		                        _( "F5" ),
		                        _( "F6" ),
		                        _( "F7" ),
		                        _( "F8" ),
		                        _( "F9" ),
		                        _( "F10" ),
		                        _( "F11" ),
		                        _( "F12" ),
		                        _( "F13" ),
		                        _( "F14" ),
		                        _( "F15" ),
		                        _( "F16" ),
		                        _( "F17" ),
		                        _( "F18" ),
		                        _( "F19" ),
		                        _( "F20" ),
		                        _( "F21" ),
		                        _( "F22" ),
		                        _( "F23" ),
		                        _( "F24" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "NUMLOCK" ),
		                        _( "SCROLL" ),
		                        _( "OEM_NEC_EQUAL" ),
		                        _( "OEM_FJ_MASSHOU" ),
		                        _( "OEM_FJ_TOUROKU" ),
		                        _( "OEM_FJ_LOYA" ),
		                        _( "OEM_FJ_ROYA" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "Unknown" ),
		                        _( "LSHIFT" ),
		                        _( "RSHIFT" ),
		                        _( "LCONTROL" ),
		                        _( "RCONTROL" ),
		                        _( "LMENU" ),
		                        _( "RMENU" ) };

	return const_cast< char* >( key_names[ virtual_key ] );
}
