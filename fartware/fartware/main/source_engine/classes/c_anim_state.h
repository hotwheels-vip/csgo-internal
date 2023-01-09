#pragma once
#include "../../features/visuals/players/players.h"
#include "../../utilities/macros.h"
#include "c_vector.h"

class c_anim_state
{
	PAD( 0x4 );
	bool first_run_since_init;
	PAD( 0x5b );
	c_base_entity* player;
	c_weapon_data* weapon;
	c_weapon_data* weapon_last;
	float last_update_time;
	int last_update_frame;
	float last_update_increment;
	float eye_yaw;
	float eye_pitch;
	float foot_yaw;
	float foot_yaw_last;
	float move_yaw;
	float move_yaw_ideal;
	float move_yaw_current_to_ideal;
	float time_to_align_lower_body;
	float primary_cycle;
	float move_weight;
	float move_weight_smoothed;
	float anim_duck_amount;
	float duck_additional;
	float recrouch_weight;
	c_vector position_current;
	c_vector position_last;
	c_vector velocity;
	c_vector velocity_normalized;
	c_vector velocity_normalized_non_zero;
	float velocity_length_xy;
	float velocity_length_z;
	float speed_as_portion_of_run_top_speed;
	float speed_as_portion_of_walk_top_speed;
	float speed_as_portion_of_crouch_top_speed;
	float duration_moving;
	float duration_still;
	bool on_ground;
	bool landing;
	PAD( 0x12 );
	float walk_to_run_transition;
	PAD( 0x4 );
	float in_air_smooth_value;
	bool on_ladder;
	PAD( 0x3 );
	float ladder_weight;
	PAD( 0x2c );
	c_vector velocity_last;
	PAD( 0x45 );
	bool player_is_accelerating;
	PAD( 0x2 );
	void* pose_param_mappings[ 20 ];
	float duration_move_weight_is_too_high;
	float static_approach_speed;
	int previous_move_state;
	float stutter_step;
	PAD( 0x80 );
	float aim_yaw_min;
	float aim_yaw_max;
	float aim_pitch_min;
	float aim_pitch_max;
	int animstate_version;
};
