#pragma once

namespace console
{
	enum class log_level
	{
		NORMAL = 0,
		DEBUG,
		SUCCESS,
		WARNING,
		FATAL
	};

	enum log_colors : int
	{
		BLACK     = 0,
		DARK_BLUE = 1,
		DARK_GREEN,
		DARK_CYAN,
		DARK_RED,
		DARK_PURPLE,
		DARK_YELLOW,
		DARK_WHITE,
		GRAY,
		BLUE,
		GREEN,
		CYAN,
		RED,
		PURPLE,
		YELLOW,
		WHITE
	};
} // namespace console
