
#pragma once

#include "../../../globals/macros/macros.h"
#include "../../../utils/math/types/vector.h"
#include "../../../utils/vfunc/vfunc.h"
#include "c_usercmd.h"
#include "c_verified_cmd.h"

#include <iostream>

constexpr std::size_t multiplayer_backup = 150;

namespace sdk
{
	class c_input
	{
	public:
		PAD( 0xC );
		bool trackir_available;
		bool mouse_initialized;
		bool mouse_active;
		PAD( 0x9A );
		bool camera_in_third_person;
		PAD( 0x2 );
		math::vec3 camera_offset;
		PAD( 0x38 );
		sdk::c_user_cmd* commands;
		sdk::c_verified_cmd* verified_commands;

		inline sdk::c_user_cmd* get_user_cmd( const int sequence_number );
		inline sdk::c_verified_cmd* get_verified_cmd( const int sequence_number );
	};
} // namespace sdk

sdk::c_user_cmd* sdk::c_input::get_user_cmd( const int sequence_number )
{
	return &commands[ sequence_number % multiplayer_backup ];
}

sdk::c_verified_cmd* sdk::c_input::get_verified_cmd( const int sequence_number )
{
	return &verified_commands[ sequence_number % multiplayer_backup ];
}
