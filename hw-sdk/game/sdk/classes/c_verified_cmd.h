#pragma once

#include "c_usercmd.h"
#include "../../../dependencies/checksum/checksum.hpp"

namespace sdk
{
	class c_verified_cmd
	{
	public:
		sdk::c_user_cmd command;
		CRC32_t checksum;
	};
} // namespace sdk
