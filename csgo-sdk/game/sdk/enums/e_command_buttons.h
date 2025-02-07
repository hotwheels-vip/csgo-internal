#pragma once

enum e_command_buttons : int {
	in_attack         = ( 1 << 0 ),
	in_jump           = ( 1 << 1 ),
	in_duck           = ( 1 << 2 ),
	in_forward        = ( 1 << 3 ),
	in_back           = ( 1 << 4 ),
	in_use            = ( 1 << 5 ),
	in_cancel         = ( 1 << 6 ),
	in_left           = ( 1 << 7 ),
	in_right          = ( 1 << 8 ),
	in_moveleft       = ( 1 << 9 ),
	in_moveright      = ( 1 << 10 ),
	in_second_attack  = ( 1 << 11 ),
	in_run            = ( 1 << 12 ),
	in_reload         = ( 1 << 13 ),
	in_left_alt       = ( 1 << 14 ),
	in_right_alt      = ( 1 << 15 ),
	in_score          = ( 1 << 16 ),
	in_speed          = ( 1 << 17 ),
	in_walk           = ( 1 << 18 ),
	in_zoom           = ( 1 << 19 ),
	in_first_weapon   = ( 1 << 20 ),
	in_second_weapon  = ( 1 << 21 ),
	in_bullrush       = ( 1 << 22 ),
	in_first_grenade  = ( 1 << 23 ),
	in_second_grenade = ( 1 << 24 ),
	in_middle_attack  = ( 1 << 25 ),
	in_use_or_reload  = ( 1 << 26 )
};
