#pragma once
#include <cstdint>

namespace sdk
{
	struct demo_playback_parameters_t {
		std::uint64_t case_id;
		std::uint32_t header_prefix_length;
		std::uint32_t lock_first_person_account_id;
		bool anonymous_player_identity;
		std::uint32_t round_skip;
		std::uint32_t round_stop;
		bool skip_warmup;
		bool playing_live_remote_broadcast;
		std::uint64_t live_match_id;
	};
} // namespace sdk
