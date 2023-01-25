#pragma once

struct aimbot_t {
	void on_create_move_post( );

private:
	bool is_weapon_valid( );
};

inline aimbot_t aimbot = { };