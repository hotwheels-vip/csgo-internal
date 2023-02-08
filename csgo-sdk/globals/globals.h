#pragma once

class c_user_cmd;
class c_base_entity;

namespace n_globals
{
	struct impl_t {
		c_user_cmd* cmd      = nullptr;
		c_base_entity* local = nullptr;
	};

} // namespace n_globals

inline n_globals::impl_t g_globals;
