#pragma once
#include "../../game/sdk/classes/i_cvar.h"
#include "../../globals/includes/includes.h"
#include "../../globals/interfaces/interfaces.h"

namespace convars
{
	struct impl {
	public:
		sdk::con_var* operator[]( const char* convar_name )
		{
			auto pointer = g_interfaces.convar->find_var( convar_name );

			return pointer;
		}
	};
} // namespace convars

inline convars::impl g_convars;
