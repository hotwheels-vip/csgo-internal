#pragma once
#include "detour_hook/c_detour_hook.h"


namespace n_hooks
{
	struct impl_t {
		bool on_attach( );
		void on_release( );

		c_detour_hook m_alloc_key_values_memory{ };
		c_detour_hook m_create_move_proxy{ };
	};
} // namespace n_hooks

inline n_hooks::impl_t g_hooks{ };

namespace n_detoured_functions
{
	void* __fastcall alloc_key_values_memory( void* ecx, void* edx, int size );
	void __fastcall create_move_proxy( void* thisptr, int edx, int sequence_number, float input_sample_frametime, bool is_active );
}