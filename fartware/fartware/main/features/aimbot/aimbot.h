#pragma once
class c_base_entity;
class c_cconvar;

struct aimbot_t {
	void on_create_move_post( );

private:
	c_base_entity* find_closest( c_cconvar* recoil_scale );
	bool is_weapon_valid( c_base_entity* weapon );
};

inline aimbot_t aimbot = { };