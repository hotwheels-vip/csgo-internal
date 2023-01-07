#pragma once

#include <iostream>

#include "../../globals/includes/includes.h"

#include "../../game/sdk/structs/dt_recv.h"
#include "../../globals/interfaces/interfaces.h"

#include "../../game/sdk/structs/data_map.h"
#include "../../game/sdk/enums/td_offset.h"

namespace netvar
{
	// sdk::recv_var_proxy_fn find_proxy_table( sdk::recv_table* table, const char* var, sdk::recv_var_proxy_fn function );
	// sdk::recv_var_proxy_fn change_proxy( const char* table, const char* var, sdk::recv_var_proxy_fn function );

	std::uintptr_t get_offset( sdk::recv_table* table, const char* var );
	std::uintptr_t get_table( const char* table, const char* var );
	std::uintptr_t get_table( sdk::data_map* table, const char* var );
} // namespace netvar
