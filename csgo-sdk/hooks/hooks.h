#pragma once
#include "detour_hook/c_detour_hook.h"

#include <d3d9.h>

class c_vector;

namespace n_hooks
{
	struct impl_t {
		bool on_attach( );
		void on_release( );

		c_detour_hook m_alloc_key_values_memory{ };
		c_detour_hook m_create_move_proxy{ };
		c_detour_hook m_run_command{ };
		c_detour_hook m_emit_sound{ };
		c_detour_hook m_frame_stage_notify{ };
		c_detour_hook m_paint_traverse{ };
		c_detour_hook m_on_add_entity{ };
		c_detour_hook m_on_remove_entity{ };
		c_detour_hook m_level_init_pre_entity{ };
		c_detour_hook m_level_shutdown{ };

		c_detour_hook m_lock_cursor{ };
		c_detour_hook m_reset{ };
		c_detour_hook m_end_scene{ };
	};
} // namespace n_hooks

inline n_hooks::impl_t g_hooks{ };

namespace n_detoured_functions
{
	void* __fastcall alloc_key_values_memory( void* ecx, void* edx, int size );
	void __fastcall create_move_proxy( void* ecx, void* edx, int sequence_number, float input_sample_frametime, bool is_active );
	void __fastcall run_command( void* ecx, void* edx, void* entity, void* cmd, void* move_helper );
	void __stdcall emit_sound( void* filter, int idx, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample,
	                           float volume, int seed, float attenuation, int flags, int pitch, const c_vector* origin, const c_vector* direction,
	                           void* vec_origins, bool update_pos, float soundtime, int speakerentity, int unk );
	void __fastcall frame_stage_notify( void* ecx, void* edx, int stage );
	void __fastcall paint_traverse( void* ecx, void* edx, unsigned int panel, bool force_repaint, bool force );
	void __fastcall on_add_entity( void* ecx, void* edx, void* handle_entity, unsigned int entity_handle );
	void __fastcall on_remove_entity( void* ecx, void* edx, void* handle_entity, unsigned int entity_handle );
	void __stdcall level_init_pre_entity( const char* map_name );
	void __fastcall level_shutdown( void* thisptr );

	void __fastcall lock_cursor( void* ecx, void* edx );
	long __stdcall reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters );
	long __stdcall end_scene( IDirect3DDevice9* device );
	long __stdcall wndproc( HWND window, unsigned int message, unsigned int wide_parameter, long long_parameter );
} // namespace n_detoured_functions