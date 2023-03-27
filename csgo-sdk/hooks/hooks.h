#pragma once
#include "detour_hook/c_detour_hook.h"

#include <cstdint>
#include <d3d9.h>
#include <stdint.h>

class c_vector;
class c_view_setup;
class c_material;
class c_animation_state;
class c_base_entity;
class c_move_data;
class c_net_channel;
class c_net_message;
class bf_write;

struct game_event_t;
struct model_render_info_t;
struct matrix3x4_t;

enum e_glow_style;

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
		c_detour_hook m_get_vcollide{ };
		c_detour_hook m_particle_collection_simulate{ };
		c_detour_hook m_find_material{ };
		c_detour_hook m_modify_eye_position{ };
		c_detour_hook m_override_mouse_input{ };
		c_detour_hook m_glow_effect_spectator{ };
		c_detour_hook m_process_movement{ };
		c_detour_hook m_fire_event_intern{ };
		c_detour_hook m_net_earliertempents{ };
		c_detour_hook m_draw_set_color{ };
		c_detour_hook m_level_init_post_entity{ };
		c_detour_hook m_set_image_data_r8g8b8a8{ };
		c_detour_hook m_draw_model_execute{ };
		c_detour_hook m_list_leaves_in_box{ };
		c_detour_hook m_send_datagram{ };
		c_detour_hook m_draw_view_models{ };
		c_detour_hook m_is_hltv{ };
		c_detour_hook m_is_paused{ };
		c_detour_hook m_is_playing_demo{ };
		c_detour_hook m_send_net_msg{ };
		c_detour_hook m_draw_static_prop_array_fast{ };

		/* hook last, as we want the menu to initialise when the cheat has initialised */
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
	void __fastcall level_init_post_entity( void* ecx, void* edx );
	void __fastcall level_shutdown( void* thisptr );
	void* __fastcall get_vcollide( void* ecx, void* edx, int model_index );
	void __fastcall particle_collection_simulate( void* ecx, void* edx );
	c_material* __fastcall find_material( void* ecx, void* edx, const char* material_name, const char* texture_group, bool complain,
	                                      const char* complain_prefix );
	void __fastcall modify_eye_position( c_animation_state* anim_state, void* edx, c_vector& input_eye_pos );
	void __fastcall override_mouse_input( void* thisptr, int edx, float* x, float* y );
	void __fastcall lock_cursor( void* ecx, void* edx );
	long __stdcall reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters );
	long __stdcall end_scene( IDirect3DDevice9* device );
	long __stdcall wndproc( HWND window, unsigned int message, unsigned int wide_parameter, long long_parameter );
	bool __cdecl glow_effect_spectator( c_base_entity* player, c_base_entity* local, e_glow_style& style, c_vector& glow_color, float& alpha_start,
	                                    float& alpha, float& time_start, float& time_target, bool& animate );
	void __fastcall process_movement( void* thisptr, void* edx, c_base_entity* player, c_move_data* move_data );
	bool __fastcall fire_event_intern( void* ecx, void* edx, game_event_t* game_event );
	void __stdcall draw_set_color( int r, int g, int b, int a );
	bool __fastcall net_earliertempents( void* ecx, void* edx );
	bool __fastcall set_image_data_r8g8b8a8( void* ecx, void* edx, const uint8_t* data, uint32_t len, const char* filename, int w, int h, void* arg1,
	                                         int arg2 );
	void __fastcall draw_model_execute( void* ecx, void* edx, void* context, void* state, model_render_info_t& info,
	                                    matrix3x4_t* custom_bone_to_world );
	int __fastcall list_leaves_in_box( void* ecx, void* edx, const c_vector& mins, const c_vector& maxs, unsigned short* list, int list_max );
	int __fastcall send_datagram( c_net_channel* net_channel, int edx, bf_write* datagram );
	void __fastcall draw_view_models( void* ecx, void* edx, c_view_setup& setup, bool draw_view_model, bool draw_scope_lens_mask );
	bool __cdecl is_hltv( void* ecx, void* edx );
	bool __cdecl is_paused( void* ecx, void* edx );
	bool __fastcall is_playing_demo( void* ecx, void* edx );
	bool __fastcall send_net_msg( void* ecx, void* edx, c_net_message* message, bool force_reliable, bool voice );
	int __fastcall draw_static_prop_array_fast( void* ecx, void* edx, void* props, int count, bool shadow_depth );

} // namespace n_detoured_functions
