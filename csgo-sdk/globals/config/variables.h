#pragma once
#include "../../game/sdk/classes/c_color.h"
#include "config.h"

namespace n_variables
{
	struct impl_t {
		/* menu */
		ADD_VARIABLE( c_color, m_accent, c_color( 129, 99, 251, 255 ) );
	};
}; // namespace n_variables

inline n_variables::impl_t g_variables{ };