#pragma once
#include <iostream>

namespace sdk
{
	enum class teams : std::int32_t
	{
		TEAM_UNKNOWN,
		TEAM_SPECTATOR,
		TEAM_TERRORIST,
		TEAM_COUNTER_TERRORIST,
	};
} // namespace sdk
