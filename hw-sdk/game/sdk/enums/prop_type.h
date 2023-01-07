#pragma once

#include <cstdint>

namespace sdk
{
	enum class prop_type : std::uint32_t
	{
		INT = 0,
		FLOAT,
		VECTOR,
		VECTOR2D,
		STRING,
		ARRAY,
		DATATABLE,
		INT64,
		SENDPROPTYPEMAX
	};
} // namespace sdk
