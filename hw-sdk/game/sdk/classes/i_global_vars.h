#pragma once
#include "../../../utils/helpers/primitive_definitions/primitives.h"

namespace sdk
{
	class i_global_vars_base // 0x3E
	{
	public:
		float real_time;
		int frame_count;
		float abs_frame_time;
		float abs_frame_start_time;
		float current_time;
		float frame_time;
		float max_clients;
		int tick_count;
		float interval_per_tick;
		float interpolation_amount;
		float frame_simulation_ticks;
		int network_protocol;
		unk save_data;
		bool client;
		bool remote_client;

	private:
		int __pad0;
		int __pad1;
	};
	class i_global_vars : public sdk::i_global_vars_base // 0x62
	{
	public:
		std::string map_name;
		std::string map_group_name;
		int map_version;
		std::string start_spot;
		int load_type;
		bool map_load_failed;
		bool deathmatch;
		bool cooperative;
		bool team_play;
		int max_entities;
		int server_count;
		void* edicts;
	};
} // namespace sdk
