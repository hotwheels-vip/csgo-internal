#pragma once

#include "../../../globals/includes/includes.h"

#include "../structs/i_material.h"
#include "../structs/i_model.h"
#include "../structs/studio.h"
#include "i_client_renderable.h"

namespace sdk
{
	class iv_model_info
	{
	public:
		virtual ~iv_model_info( ) { }
		virtual const i_model* get_model( int model_index ) const                                              = 0;
		virtual int get_model_index( const char* name ) const                                                  = 0;
		virtual const char* get_model_name( const i_model* model ) const                                       = 0;
		virtual void** get_v_collide( const i_model* model ) const                                             = 0;
		virtual void** get_v_collide( int model_index ) const                                                  = 0;
		virtual void** get_v_collide( const i_model* model, float fl_scale ) const                             = 0;
		virtual void** get_v_collide( int model_index, float fl_scale ) const                                  = 0;
		virtual void get_model_bounds( const i_model* model, math::vec3& mins, math::vec3& maxs ) const        = 0;
		virtual void get_model_render_bounds( const i_model* model, math::vec3& mins, math::vec3& maxs ) const = 0;
		virtual int get_model_frame_count( const i_model* model ) const                                        = 0;
		virtual int get_i_modelype( const i_model* model ) const                                               = 0;
		virtual void* get_model_extra_data( const i_model* model )                                             = 0;
		virtual bool model_has_material_proxy( const i_model* model ) const                                    = 0;
		virtual bool is_translucent( i_model const* model ) const                                              = 0;
		virtual bool is_translucent_two_pass( const i_model* model ) const                                     = 0;

	private:
		virtual void padding( ) = 0;

	public:
		virtual void* compute_translucency_type( const i_model* model, int skin, int body )                                   = 0;
		virtual int get_model_material_count( const i_model* model ) const                                                    = 0;
		virtual void get_model_materials( const i_model* model, int i_count, i_material** materials )                         = 0;
		virtual bool is_model_vertex_lit( const i_model* model ) const                                                        = 0;
		virtual const char* get_model_key_value_text( const i_model* model )                                                  = 0;
		virtual bool get_model_key_value( const i_model* model, void*& buf )                                                  = 0;
		virtual float get_model_radius( const i_model* model )                                                                = 0;
		virtual const void** find_model( const void** studio_hdr, void** cache, const char* model_name ) const                = 0;
		virtual const void** find_model( void* cache ) const                                                                  = 0;
		virtual void** get_virtual_model( const void** studio_hdr ) const                                                     = 0;
		virtual std::uint8_t* get_anim_block( const void** studio_hdr, int i_block ) const                                    = 0;
		virtual bool has_anim_block_beepreloaded( void* const* studio_hdr, int i_block ) const                                = 0;
		virtual void get_model_material_color_and_lighting( const i_model* model, const math::vec3& vec_origin, const math::vec3& angles,
		                                                    trace_t* trace, math::vec3& vec_lighting, math::vec3& mat_color ) = 0;
		virtual void get_illuminatiopoint( const i_model* model, i_client_renderable* renderable, const math::vec3& vec_origin,
		                                   const math::vec3& angles, math::vec3* lighting_center )                            = 0;
		virtual int get_model_contents( int model_index ) const                                                               = 0;

	private:
		virtual void padding_( ) = 0;

	public:
		virtual int get_model_sprite_width( const i_model* model ) const                                                              = 0;
		virtual int get_model_sprite_height( const i_model* model ) const                                                             = 0;
		virtual void set_level_screefade_range( float fl_misize, float fl_max_size )                                                  = 0;
		virtual void get_level_screefade_range( float* miarea, float* max_area ) const                                                = 0;
		virtual void set_view_screefade_range( float fl_misize, float fl_max_size )                                                   = 0;
		virtual unsigned char compute_level_screefade( const math::vec3& vec_abs_origin, float fl_radius, float fl_fade_scale ) const = 0;
		virtual unsigned char compute_view_screefade( const math::vec3& vec_abs_origin, float fl_radius, float fl_fade_scale ) const  = 0;
		virtual int get_autoplay_list( const void** studio_hdr, unsigned short** autoplay_list ) const                                = 0;
		virtual void** get_collide_for_virtual_terrain( int index )                                                                   = 0;
		virtual bool is_using_fb_texture( const i_model* model, int skin, int body, i_client_renderable* client_renderable ) const    = 0;
		virtual const i_model* find_or_load_model( const char* name ) const                                                           = 0;
		virtual void* get_cache_handle( const i_model* model ) const                                                                  = 0;
		virtual int get_brush_model_plane_count( const i_model* model ) const                                                         = 0;
		virtual void get_brush_model_plane( const i_model* model, int index, void*& plane, math::vec3* origin ) const                 = 0;
		virtual int get_surfaceprops_for_virtual_terrain( int index )                                                                 = 0;
		virtual bool uses_env_cubemap( const i_model* model ) const                                                                   = 0;
		virtual bool uses_static_lighting( const i_model* model ) const                                                               = 0;
		virtual int register_dynamic_model( const char* name, bool b_client_side )                                                    = 0;
		virtual int register_combined_dynamic_model( const char* name, void* handle )                                                 = 0;
		virtual void update_combined_dynamic_model( int model_index, void* handle )                                                   = 0;
		virtual int begicombined_model( const char* name, bool b_reuse_existing )                                                     = 0;

		studio_hdr* get_studio_model( i_model* model )
		{
			return virtual_func::call< sdk::studio_hdr* >( this, 32, model );
		}
	};
} // namespace sdk
