#pragma once
#include <iostream>

#include "../../../globals/macros/macros.h"

namespace sdk
{
	struct cnetmsg_signonstate_t {
	public:
		PAD( 12 );
		std::int32_t signon_state;
		std::int32_t spawn_count;
		std::int32_t num_server_players;
		/* |->*/ wchar_t players_networkids[ 2 ];
		/* |->*/ wchar_t map_name[ 2 ];
		// | These aren't the real values. I just don't know what a protobuf's string class is.
	};
} // namespace sdk
