#pragma once
#include "../lagcomp/lagcomp.h"
#include <array>

class c_base_entity;

namespace n_aimbot
{
	struct impl_t {
		void on_create_move_post( );

	private:
		// find closest player within the given method
		// TODO: add these methods for scanning;
		//		- distance
		//		- best simtime(maybe?)
		//		- best hitchance

		c_base_entity* find_closest_player( );

		// TODO: add this, use code from hw-sdk
		float hitchance( c_base_entity* player, c_vector angles, int hit_group );
	};
} // namespace n_aimbot

inline n_aimbot::impl_t g_aimbot{ };
