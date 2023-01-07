#pragma once
#include "../../../globals/macros/macros.h"
#include <cstdint>

namespace sdk
{
	struct weapon_data_t {
		PAD( 0x14 );
		int max_clip1;
		int max_clip2;
		int default_clip1;
		int default_clip2;
		int primary_max_reserve_ammo;
		int secondary_max_reserve_ammo;
		const char* world_model;
		const char* view_model;
		const char* dropped_model;
		PAD( 0x50 );
		const char* hud_name;
		const char* weapon_name;
		PAD( 0x2 );
		bool is_melee_weapon;
		PAD( 0x9 );
		float weapon_weight;
		PAD( 0x4 );
		int slot;
		int position;
		PAD( 0x1c );
		int weapon_type;
		PAD( 0x4 );
		int weapon_price;
		int kill_award;
		const char* anim_prefix;
		float cycle_time;
		float cycle_time_alt;
		PAD( 0x8 );
		bool full_auto;
		PAD( 0x3 );
		int damage;
		float armor_ratio;
		float head_shot_multiplier;
		int bullets;
		float penetration;
		PAD( 0x8 );
		float range;
		float range_modifier;
		float throw_velocity;
		PAD( 0xC );
		bool has_silencer;
		PAD( 0xf );
		float max_speed[ 2 ];
		PAD( 0x4 );
		float spread[ 2 ];
		float innacuracy_crouch[ 2 ];
		float innacuracy_stand[ 2 ];
		PAD( 0x8 );
		float innacuracy_jump[ 2 ];
		float innacuracy_land[ 2 ];
		float innacuracy_ladder[ 2 ];
		float innacuracy_fire[ 2 ];
		float innacuracy_move[ 2 ];
		float innacuracy_reload;
		int recoil_seed;
		float recoil_angle[ 2 ];
		float recoil_angle_variance[ 2 ];
		float recoil_magnitude[ 2 ];
		float recoil_magnitude_variance[ 2 ];
		int spread_seed;
	};
} // namespace sdk
