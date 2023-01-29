#pragma once

#include "../../dependencies/minhook/include/MinHook.h"

#include "../includes.h"
#include "../source_engine/classes/c_view_setup.h"

class c_detour_hook
{
public:
	c_detour_hook( ) = default;

	explicit c_detour_hook( void* function, void* detour ) : m_base_fn( function ), m_replace_fn( detour ) { }

	bool create( void* function, void* detour )
	{
		m_base_fn = function;

		if ( !m_base_fn )
			return false;

		m_replace_fn = detour;

		if ( !m_replace_fn )
			return false;

		const MH_STATUS status = MH_CreateHook( m_base_fn, m_replace_fn, &m_original_fn );

		if ( status != MH_OK )
			throw std::runtime_error(
				std::vformat( ( "failed to create hook function, status: {}\nbase function -> {:#08X}" ),
			                  std::make_format_args( MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( m_base_fn ) ) ) );

		if ( !this->replace( ) )
			return false;

		return true;
	}

	bool replace( )
	{
		if ( m_base_fn == nullptr )
			return false;

		if ( m_is_hooked )
			return false;

		const MH_STATUS status = MH_EnableHook( m_base_fn );

		if ( status != MH_OK )
			throw std::runtime_error(
				std::vformat( ( "failed to enable hook function, status: {}\nbase function -> {:#08X} address" ),
			                  std::make_format_args( MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( m_base_fn ) ) ) );

		m_is_hooked = true;
		return true;
	}

	template< typename fn >
	fn get_original( )
	{
		return static_cast< fn >( m_original_fn );
	}

private:
	bool m_is_hooked    = false;
	void* m_base_fn     = nullptr;
	void* m_replace_fn  = nullptr;
	void* m_original_fn = nullptr;
};

struct hooks_t {
	bool on_attach( );

	inline void on_release( )
	{
		// std::free( globals.backup.m_backup_cmd );
		// std::free( globals.backup.m_backup_local );

		MH_DisableHook( MH_ALL_HOOKS );
		MH_RemoveHook( MH_ALL_HOOKS );
		MH_Uninitialize( );
	}

	c_detour_hook alloc_key_values_memory = { };
	c_detour_hook create_move_proxy       = { };
	c_detour_hook frame_stage_notify      = { };
	c_detour_hook on_add_entity           = { };
	c_detour_hook on_remove_entity        = { };
	c_detour_hook level_init_pre_entity   = { };
	c_detour_hook level_shutdown          = { };
	c_detour_hook paint_traverse          = { };
	c_detour_hook lock_cursor             = { };
	c_detour_hook reset                   = { };
	c_detour_hook end_scene               = { };
	c_detour_hook draw_print_text         = { };
	c_detour_hook vsnprintf               = { };
	c_detour_hook emit_sound              = { };
	c_detour_hook override_mouse_input    = { };
	c_detour_hook modify_eye_position     = { };
	c_detour_hook draw_set_color          = { };
	c_detour_hook draw_model_execute      = { };
	c_detour_hook glow_effect_spectator   = { };
	c_detour_hook draw_view_models        = { };
	c_detour_hook get_vcollide        = { };
	c_detour_hook on_full_update        = { };
};

inline hooks_t hooks = { };

namespace n_detoured_functions
{
	void* __fastcall alloc_key_values_memory( c_key_values_system* thisptr, int edx, int size );
	// bool __stdcall create_move( float input_sample_time, c_user_cmd* cmd );
	void __fastcall create_move_proxy( void* thisptr, int edx, int sequence_number, float input_sample_frametime, bool is_active );
	void __stdcall emit_sound( void* filter, int idx, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample,
	                           float volume, int seed, float attenuation, int flags, int pitch, const c_vector* origin, const c_vector* direction,
	                           void* utl_vec_origins, bool update_pos, float soundtime, int speakerentity, int unk );
	void __fastcall frame_stage_notify( void* thisptr, int edx, e_client_frame_stage stage );
	void __fastcall on_add_entity( int ecx, int edx, void* handle_entity, unsigned int entity_handle );
	void __fastcall on_remove_entity( int ecx, int edx, void* handle_entity, unsigned int entity_handle );
	void __stdcall level_init_pre_entity( const char* map_name );
	void __fastcall level_shutdown( void* thisptr );
	void __fastcall paint_traverse( c_surface* thisptr, int edx, unsigned int panel, bool force_repaint, bool force );
	void __fastcall lock_cursor( void* thisptr, int edx );
	long __stdcall reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters );
	long __stdcall end_scene( IDirect3DDevice9* device );
	void __fastcall draw_print_text( int ecx, int edx, const wchar_t* text, int text_length, int draw_type );
	int __cdecl vsnprintf( char* dest, int text_length, const char* fmt, ... );
	void __fastcall override_mouse_input( void* thisptr, int edx, float* x, float* y );
	void __fastcall modify_eye_position( c_anim_state* anim_state, void* edx, c_vector& input_eye_pos );
	void __stdcall draw_set_color( int r, int g, int b, int a );
	void __fastcall draw_model_execute( int ecx, int edx, void* context, void* state, model_render_info_t* info, matrix3x4_t* bone_to_world );
	bool __cdecl glow_effect_spectator( c_base_entity* player, c_base_entity* local, e_glow_style& style, c_vector& glow_color, float& alpha_start,
	                                    float& alpha, float& time_start, float& time_target, bool& animate );
	void __fastcall draw_view_models( void* thisptr, void* edx, c_view_setup& setup, bool draw_view_model, bool draw_scope_lens_mask );
	void* __fastcall get_vcollide( void* ecx, void* edx, unsigned int model );
	void __fastcall on_full_update( void* ecx, void* edx, const char* text );

	long __stdcall wndproc( HWND window, UINT msg, WPARAM wide_param, LPARAM long_param );
} // namespace n_detoured_functions
