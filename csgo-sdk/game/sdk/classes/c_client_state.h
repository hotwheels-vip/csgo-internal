#pragma once
#include "../classes/c_angle.h"

class c_net_channel;
class c_event_info;

class c_client_state
{
public:
	unsigned char pad0[ 0x9c ];
	c_net_channel* m_net_channel;
	int m_challenge_nr;
	unsigned char pad1[ 0x64 ];
	int m_signon_state;
	unsigned char pad2[ 0x8 ];
	float m_next_cmd_time;
	int m_server_count;
	int m_current_sequence;
	unsigned char pad3[ 0x54 ];
	int m_delta_tick;
	bool m_paused;
	unsigned char pad4[ 0x7 ];
	int m_view_entity;
	int m_player_slot;
	char m_level_name[ 260 /* MAX_PATH */ ];
	char m_level_name_short[ 80 ];
	char m_map_group_name[ 80 ];
	char m_last_level_name_short[ 80 ];
	unsigned char pad5[ 0xc ];
	int m_max_clients;
	unsigned char pad6[ 0x498c ];
	float m_last_server_tick_time;
	bool m_in_simulation;
	unsigned char pad7[ 0x3 ];
	int m_old_tickcount;
	float m_tick_remainder;
	float m_frame_time;
	int m_last_outgoing_command;
	int m_choked_commands;
	int m_last_command_ack;
	int m_command_ack;
	int m_sound_sequence;
	unsigned char pad8[ 0x50 ];
	c_angle m_view_point;
	unsigned char pad9[ 0xd0 ];
	c_event_info* m_events;
};