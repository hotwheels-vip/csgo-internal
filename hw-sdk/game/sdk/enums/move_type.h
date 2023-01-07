#pragma once

namespace sdk
{
	enum move_type : std::int32_t
	{
		MOVE_NONE = 0,
		MOVE_ISOMETRIC,
		MOVE_WALK,
		MOVE_STEP,
		MOVE_FLY,
		MOVE_FLYGRAVITY,
		MOVE_VPHYSICS,
		MOVE_PUSH,
		MOVE_NOCLIP,
		MOVE_LADDER,
		MOVE_OBSERVER,
		MOVE_CUSTOM,
		MOVE_LAST     = MOVE_CUSTOM,
		MOVE_MAX_BITS = 4
	};
} // namespace sdk
