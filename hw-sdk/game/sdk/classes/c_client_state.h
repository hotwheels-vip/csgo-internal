#pragma once
#include "../../../globals/macros/macros.h"
#include "../../../utils/math/types/vector.h"
#include "../structs/c_event_info.h"
#include "i_net_channel_info.h"
#include <cstdint>

namespace sdk
{
	class c_client_state
	{
	public:
		PAD( 0x9C );
		i_net_channel_info* net_channel;
		int challenge_nr;
		PAD( 0x64 );
		int signon_state;
		PAD( 0x8 );
		float next_command_time;
		int server_count;
		int current_sequence;
		PAD( 0x54 );
		int delta_tick;
		bool paused;
		PAD( 0x7 );
		int view_entity;
		int player_slot;
		char level_name[ MAX_PATH ];
		char level_name_short[ 80 ];
		char map_group_name[ 80 ];
		char last_level_name_short[ 80 ];
		PAD( 0xC );
		int max_clients;
		PAD( 0x498C );
		float last_server_tick_time;
		bool in_simulation;
		PAD( 0x3 );
		int old_tick_count;
		float tick_remainder;
		float frame_time;
		int last_outgoing_command;
		int choked_commands;
		int last_command_ack;
		int command_ack;
		int sound_sequence;
		PAD( 0x50 );
		math::vec3 view_point;
		PAD( 0xD0 );
		c_event_info* events;
	};
} // namespace sdk
