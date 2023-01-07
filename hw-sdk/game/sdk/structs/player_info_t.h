#pragma once
#include <cstdint>

namespace sdk
{
	struct player_info_t {
	private:
		std::int64_t pad;
		union {
			std::int64_t xuid;
			struct {
				int xuidlow;
				int xuidhigh;
			};
		};

	public:
		char name[ 128 ]{ };
		int user_id{ };
		char guid[ 128 ]{ };
		unsigned int friends_id;
		char friends_name[ 128 ];
		bool fake_player;
		bool is_hltv;

	private:
		unsigned int custom_files[ 4 ];
		unsigned char files_downloaded;
	};
} // namespace sdk
