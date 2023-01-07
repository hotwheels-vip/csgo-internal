#pragma once
#include "../../../utils/math/types/vector.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../enums/material_var_flags.h"

namespace sdk
{
	struct i_material_var {
		void set_vector( const math::vec3& vec )
		{
			virtual_func::call< void, const math::vec3& >( this, 11, vec );
		}

		void set_component_value( float val, float comp )
		{
			virtual_func::call< void, float, float >( this, 26, val, comp );
		}
	};

	struct i_material {
		virtual const char* get_name( ) const                                                                                  = 0;
		virtual const char* get_texture_group_name( ) const                                                                    = 0;
		virtual void* get_preview_image_properties( int* width, int* height, void** image_format, bool* is_translucent ) const = 0;
		virtual void* get_preview_image( unsigned char* data, int width, int height, void* image_format ) const                = 0;
		virtual int get_mapping_width( )                                                                                       = 0;
		virtual int get_mapping_height( )                                                                                      = 0;
		virtual int get_num_animation_frames( )                                                                                = 0;
		virtual bool in_material_page( void )                                                                                  = 0;
		virtual void get_material_offset( float* p_offset )                                                                    = 0;
		virtual void get_material_scale( float* p_scale )                                                                      = 0;
		virtual i_material* get_material_page( void )                                                                          = 0;
		virtual i_material_var* find_var( const char* var_name, bool* found, bool complain = true )                            = 0;
		virtual void increment_reference_count( void )                                                                         = 0;
		virtual void decrement_reference_count( void )                                                                         = 0;
		virtual int get_enumeration_id( void ) const                                                                           = 0;
		virtual void get_low_res_color_sample( float s, float t, float* color ) const                                          = 0;
		virtual void recompute_state_snapshots( )                                                                              = 0;
		virtual bool is_translucent( )                                                                                         = 0;
		virtual bool is_alpha_tested( )                                                                                        = 0;
		virtual bool is_vertex_lit( )                                                                                          = 0;
		virtual void* get_vertex_format( ) const                                                                               = 0;
		virtual bool has_proxy( void ) const                                                                                   = 0;
		virtual bool uses_env_cubemap( void )                                                                                  = 0;
		virtual bool needs_tangent_space( void )                                                                               = 0;
		virtual bool needs_power_of_two_frame_buffer_texture( bool b_check_specific_to_this_frame = true )                     = 0;
		virtual bool needs_full_frame_buffer_texture( bool b_check_specific_to_this_frame = true )                             = 0;
		virtual bool needs_software_skinning( void )                                                                           = 0;
		virtual void alpha_modulate( float alpha )                                                                             = 0;
		virtual void color_modulate( float r, float g, float b )                                                               = 0;
		virtual void set_material_var_flag( material_var_flags flag, bool on )                                                 = 0;
		virtual bool get_material_var_flag( material_var_flags flag ) const                                                    = 0;
		virtual void get_reflectivity( math::vec3& reflect )                                                                   = 0;
		virtual bool get_property_flag( void* type )                                                                           = 0;
		virtual bool is_two_sided( )                                                                                           = 0;
		virtual void set_shader( const char* p_shader_name )                                                                   = 0;
		virtual int get_num_passes( void )                                                                                     = 0;
		virtual int get_texture_memory_bytes( void )                                                                           = 0;
		virtual void refresh( )                                                                                                = 0;
		virtual bool needs_lightmap_blend_alpha( void )                                                                        = 0;
		virtual bool needs_software_lighting( void )                                                                           = 0;
		virtual int shader_param_count( ) const                                                                                = 0;
		virtual i_material_var** get_shader_params( void )                                                                     = 0;
		virtual bool is_error_material( ) const                                                                                = 0;
		virtual void unused( )                                                                                                 = 0;
		virtual float get_alpha_modulation( )                                                                                  = 0;
		virtual void get_color_modulation( float* r, float* g, float* b )                                                      = 0;
		virtual bool is_translucent_under_modulation( float f_alpha_modulation = 1.0f ) const                                  = 0;
		virtual i_material_var* find_var_fast( char const* p_var_name, unsigned int* p_token )                                 = 0;
		virtual void set_shader_and_params( void** p_key_values )                                                              = 0;
		virtual const char* get_shader_name( ) const                                                                           = 0;
		virtual void delete_if_unreferenced( )                                                                                 = 0;
		virtual bool is_sprite_card( )                                                                                         = 0;
		virtual void call_bind_proxy( void* proxy_data )                                                                       = 0;
		virtual void refresh_preserving_material_vars( )                                                                       = 0;
		virtual bool was_reloaded_from_whitelist( )                                                                            = 0;
		virtual bool set_temp_excluded( bool b_set, int n_excluded_dimension_limit )                                           = 0;
		virtual int get_reference_count( ) const                                                                               = 0;
	};
} // namespace sdk
