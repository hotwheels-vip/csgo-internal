#pragma once
#include "../structs/matrix_t.h"
#include "c_angle.h"
#include "c_engine_trace.h"
#include "c_vector.h"

enum e_bone_index : int {
	bone_invalid = -1,
	bone_pelvis,
	bone_lean_root,
	bone_cam_driver,
	bone_spine_0,
	bone_spine_1,
	bone_spine_2,
	bone_spine_3,
	bone_neck,
	bone_head,
	bone_clavicle_l,
	bone_arm_upper_l,
	bone_arm_lower_l,
	bone_hand_l,
	bone_finger_middle_meta_l,
	bone_finger_middle_0_l,
	bone_finger_middle_1_l,
	bone_finger_middle_2_l,
	bone_finger_pinky_meta_l,
	bone_finger_pinky_0_l,
	bone_finger_pinky_1_l,
	bone_finger_pinky_2_l,
	bone_finger_index_meta_l,
	bone_finger_index_0_l,
	bone_finger_index_1_l,
	bone_finger_index_2_l,
	bone_finger_thumb_0_l,
	bone_finger_thumb_1_l,
	bone_finger_thumb_2_l,
	bone_finger_ring_meta_l,
	bone_finger_ring_0_l,
	bone_finger_ring_1_l,
	bone_finger_ring_2_l,
	bone_weapon_hand_l,
	bone_arm_lower_l_twist,
	bone_arm_lower_l_twist1,
	bone_arm_upper_l_twist,
	bone_arm_upper_l_twist1,
	bone_clavicle_r,
	bone_arm_upper_r,
	bone_arm_lower_r,
	bone_hand_r,
	bone_finger_middle_meta_r,
	bone_finger_middle_0_r,
	bone_finger_middle_1_r,
	bone_finger_middle_2_r,
	bone_finger_pinky_meta_r,
	bone_finger_pinky_0_r,
	bone_finger_pinky_1_r,
	bone_finger_pinky_2_r,
	bone_finger_index_meta_r,
	bone_finger_index_0_r,
	bone_finger_index_1_r,
	bone_finger_index_2_r,
	bone_finger_thumb_0_r,
	bone_finger_thumb_1_r,
	bone_finger_thumb_2_r,
	bone_finger_ring_meta_r,
	bone_finger_ring_0_r,
	bone_finger_ring_1_r,
	bone_finger_ring_2_r,
	bone_weapon_hand_r,
	bone_arm_lower_r_twist,
	bone_arm_lower_r_twist1,
	bone_arm_upper_r_twist,
	bone_arm_upper_r_twist1,
	bone_leg_upper_l,
	bone_leg_lower_l,
	bone_ankle_l,
	bone_ball_l,
	bone_lfoot_lock,
	bone_leg_upper_l_twist,
	bone_leg_upper_l_twist1,
	bone_leg_upper_r,
	bone_leg_lower_r,
	bone_ankle_r,
	bone_ball_r,
	bone_rfoot_lock,
	bone_leg_upper_r_twist,
	bone_leg_upper_r_twist1,
	bone_finger_pinky_l_end,
	bone_finger_pinky_r_end,
	bone_valvebiped_weapon_bone,
	bone_lh_ik_driver,
	bone_primary_jiggle_jnt,
};

enum e_render_flags : unsigned int {
	render_flags_disable_rendering       = 0x01,
	render_flags_haschanged              = 0x02,
	render_flags_alternate_sorting       = 0x04,
	render_flags_render_with_viewmodels  = 0x08,
	render_flags_bloat_bounds            = 0x10,
	render_flags_bounds_valid            = 0x20,
	render_flags_bounds_always_recompute = 0x40,
	render_flags_is_sprite               = 0x80,
	render_flags_force_opaque_pass       = 0x100,
};

enum e_translucency_type : int {
	renderable_is_opaque = 0,
	renderable_is_translucent,
	renderable_is_two_pass
};

using mdl_handle_t = unsigned short;

struct model_t {
	unsigned char pad0[ 0x4 ];  // 0x0000
	char m_name[ 260 ];         // 0x0004
	int m_load_flags;           // 0x0108
	int m_server_count;         // 0x010c
	int m_type;                 // 0x0110
	int m_flags;                // 0x0114
	c_vector m_mins;            // 0x0118
	c_vector m_maxs;            // 0x0124
	float m_radius;             // 0x0130
	unsigned char pad1[ 0x1c ]; // 0x0134
};

class c_phys_collide;
class c_utl_buffer;
class c_client_renderable;
class c_material;
struct studiohdr_t;
struct virtualmodel_t;

class c_model_info
{
public:
	virtual ~c_model_info( ) { }
	virtual const model_t* get_model( int n_model_index ) const                                          = 0;
	virtual int get_model_index( const char* sz_name ) const                                             = 0;
	virtual const char* get_model_name( const model_t* p_model ) const                                   = 0;
	virtual void* get_v_collide( const model_t* p_model ) const                                          = 0;
	virtual void* get_v_collide( int n_model_index ) const                                               = 0;
	virtual void* get_v_collide( const model_t* p_model, float fl_scale ) const                          = 0;
	virtual void* get_v_collide( int n_model_index, float fl_scale ) const                               = 0;
	virtual void get_model_bounds( const model_t* p_model, c_vector& mins, c_vector& maxs ) const        = 0;
	virtual void get_model_render_bounds( const model_t* p_model, c_vector& mins, c_vector& maxs ) const = 0;
	virtual int get_model_frame_count( const model_t* p_model ) const                                    = 0;
	virtual int get_model_type( const model_t* p_model ) const                                           = 0;
	virtual void* get_model_extra_data( const model_t* p_model )                                         = 0;
	virtual bool model_has_material_proxy( const model_t* p_model ) const                                = 0;
	virtual bool is_translucent( model_t const* p_model ) const                                          = 0;
	virtual bool is_translucent_two_pass( const model_t* p_model ) const                                 = 0;

private:
	virtual void unused0( ) { }

public:
	virtual e_translucency_type compute_translucency_type( const model_t* p_model, int n_skin, int n_body )                            = 0;
	virtual int get_model_material_count( const model_t* p_model ) const                                                               = 0;
	virtual void get_model_materials( const model_t* p_model, int i_count, c_material** pp_materials )                                 = 0;
	virtual bool is_model_vertex_lit( const model_t* p_model ) const                                                                   = 0;
	virtual const char* get_model_key_value_text( const model_t* p_model )                                                             = 0;
	virtual bool get_model_key_value( const model_t* p_model, c_utl_buffer& buf )                                                      = 0;
	virtual float get_model_radius( const model_t* p_model )                                                                           = 0;
	virtual const studiohdr_t* find_model( const studiohdr_t* p_studio_hdr, void** pp_cache, const char* sz_model_name ) const         = 0;
	virtual const studiohdr_t* find_model( void* p_cache ) const                                                                       = 0;
	virtual virtualmodel_t* get_virtual_model( const studiohdr_t* p_studio_hdr ) const                                                 = 0;
	virtual std::uint8_t* get_anim_block( const studiohdr_t* p_studio_hdr, int i_block ) const                                         = 0;
	virtual bool has_anim_block_been_preloaded( studiohdr_t const* p_studio_hdr, int i_block ) const                                   = 0;
	virtual void get_model_material_color_and_lighting( const model_t* p_model, const c_vector& vec_origin, const c_angle& angles, trace_t* p_trace,
	                                                    c_vector& vec_lighting, c_vector& mat_color )                                  = 0;
	virtual void get_illumination_point( const model_t* p_model, c_client_renderable* p_renderable, const c_vector& vec_origin, const c_angle& angles,
	                                     c_vector* p_lighting_center )                                                                 = 0;
	virtual int get_model_contents( int n_model_index ) const                                                                          = 0;
	virtual studiohdr_t* get_studio_model( const model_t* p_model )                                                                    = 0;
	virtual int get_model_sprite_width( const model_t* p_model ) const                                                                 = 0;
	virtual int get_model_sprite_height( const model_t* p_model ) const                                                                = 0;
	virtual void set_level_screen_fade_range( float fl_min_size, float fl_max_size )                                                   = 0;
	virtual void get_level_screen_fade_range( float* p_min_area, float* p_max_area ) const                                             = 0;
	virtual void set_view_screen_fade_range( float fl_min_size, float fl_max_size )                                                    = 0;
	virtual unsigned char compute_level_screen_fade( const c_vector& vec_abs_origin, float fl_radius, float fl_fade_scale ) const      = 0;
	virtual unsigned char compute_view_screen_fade( const c_vector& vec_abs_origin, float fl_radius, float fl_fade_scale ) const       = 0;
	virtual int get_autoplay_list( const studiohdr_t* p_studio_hdr, unsigned short** p_autoplay_list ) const                           = 0;
	virtual c_phys_collide* get_collide_for_virtual_terrain( int n_index )                                                             = 0;
	virtual bool is_using_fb_texture( const model_t* p_model, int n_skin, int n_body, c_client_renderable* p_client_renderable ) const = 0;
	virtual const model_t* find_or_load_model( const char* sz_name ) const                                                             = 0;
	virtual mdl_handle_t get_cache_handle( const model_t* p_model ) const                                                              = 0;
	virtual int get_brush_model_plane_count( const model_t* p_model ) const                                                            = 0;
	virtual void get_brush_model_plane( const model_t* p_model, int n_index, cplane_t& plane, c_vector* p_origin ) const               = 0;
	virtual int get_surfaceprops_for_virtual_terrain( int n_index )                                                                    = 0;
	virtual bool uses_env_cubemap( const model_t* p_model ) const                                                                      = 0;
	virtual bool uses_static_lighting( const model_t* p_model ) const                                                                  = 0;
	virtual int register_dynamic_model( const char* name, bool b_client_side )                                                         = 0;
	virtual int register_combined_dynamic_model( const char* psz_name, mdl_handle_t handle )                                           = 0;
	virtual void update_combined_dynamic_model( int n_model_index, mdl_handle_t handle )                                               = 0;
	virtual int begin_combined_model( const char* psz_name, bool b_reuse_existing )                                                    = 0;
};
