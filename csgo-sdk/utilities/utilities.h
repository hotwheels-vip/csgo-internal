#pragma once

namespace n_utilities
{
	struct impl_t {
		int create_thread( unsigned long __stdcall function( void* ), void* parameter );
	};
} // namespace n_utilities

inline n_utilities::impl_t g_utilities;