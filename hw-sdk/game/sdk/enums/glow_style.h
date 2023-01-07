#pragma once
#include <cstdint>

namespace sdk
{
	enum glow_style : std::int32_t
	{
		GLOW_STYLE_DEFAULT = 0,
		GLOW_STYLE_RIM_3D,
		GLOW_STYLE_EDGE_HIGHLIGHT,
		GLOW_STYLE_EDGE_HIGHLIGHT_PULSE,
		GLOW_STYLE_COUNT
	};
} // namespace sdk
