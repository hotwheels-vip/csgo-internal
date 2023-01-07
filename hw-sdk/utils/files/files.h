#pragma once
#include "../../hacks/menu/config/config.h"
#include <deque>
#include <filesystem>

namespace files
{
	struct impl {
		// "%userprofile%\documents\.hw"
		std::filesystem::path get_working_path( );

		void refresh( );

		const std::filesystem::path fs_path = get_working_path( ) / _( "settings" );

		struct {
			std::deque< std::string > file_names    = { };
			std::vector< config::option > variables = { };
		} info;
	};
} // namespace files

inline files::impl g_files;
