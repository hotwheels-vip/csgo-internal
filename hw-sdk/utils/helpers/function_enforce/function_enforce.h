#pragma once
#include "../../../globals/hotwheels/hotwheels.h"
#include <string_view>

namespace function_enforce
{
	template< bool uninject, typename... args >
	inline bool enforcer( bool what, std::string_view id, std::string_view msg, args&&... arg )
	{
		constexpr auto lev = uninject ? console::log_level::FATAL : console::log_level::WARNING;

		if ( !what ) {
			std::string fmted_msg = console::format( msg, std::forward< args >( arg )... );

			console::print< lev >( _( "Enforce failed - [ id = {} -> msg = {} ]" ), id, fmted_msg );

#ifdef _DEBUG
			MessageBox( hotwheels::window, fmted_msg.c_str( ), _( "Exit failure - function enforcer" ), MB_OK );
#endif

			if constexpr ( uninject )
				hotwheels::unload( EXIT_FAILURE );
		}

		return what;
	}
}; // namespace function_enforce

// sends warning only if explicit function fails, aka if something doesnt initialize proper, it will unload/send message
// replaces assert, sends warning message
#define ENFORCE_WARNING( exp, msg, ... ) function_enforce::enforcer< false >( exp, _( #exp ), _( msg ), __VA_ARGS__ )
// sends warning message and unloads.
#define ENFORCE_FAILURE( exp, msg, ... ) function_enforce::enforcer< true >( exp, _( #exp ), _( msg ), __VA_ARGS__ )
