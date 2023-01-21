#pragma once
#include "../../utilities/mathematics/mathematics.h"

struct model_t {
	unsigned char pad0[ 0x4 ];  // 0x0000
	char m_name[ 260 ];         // 0x0004
	int m_load_flags;           // 0x0108
	int m_server_count;         // 0x010C
	int m_type;                 // 0x0110
	int m_flags;                // 0x0114
	c_vector m_mins;            // 0x0118
	c_vector m_maxs;            // 0x0124
	float m_dius;               // 0x0130
	unsigned char pad1[ 0x1C ]; // 0x0134
};

struct model_render_info_t {
	c_vector m_origin;
	c_angle m_angles;
	unsigned char pad0[ 0x4 ];
	void* m_renderable;
	const model_t* m_model;
	const matrix3x4_t* m_model_to_world;
	const matrix3x4_t* m_lighting_offset;
	const c_vector* m_lighting_origin;
	int m_flags;
	int m_entity_index;
	int m_skin;
	int m_body;
	int m_hitbox_set;
	unsigned short m_instance;
};

enum e_material_var_flags : int {
	material_var_flag_debug                    = ( 1 << 0 ),
	material_var_flag_no_debug_override        = ( 1 << 1 ),
	material_var_flag_no_draw                  = ( 1 << 2 ),
	material_var_flag_use_in_fillrate_mode     = ( 1 << 3 ),
	material_var_flag_vertexcolor              = ( 1 << 4 ),
	material_var_flag_vertexalpha              = ( 1 << 5 ),
	material_var_flag_selfillum                = ( 1 << 6 ),
	material_var_flag_additive                 = ( 1 << 7 ),
	material_var_flag_alphatest                = ( 1 << 8 ),
	material_var_flag_pseudo_translucent       = ( 1 << 9 ),
	material_var_flag_znearer                  = ( 1 << 10 ),
	material_var_flag_model                    = ( 1 << 11 ),
	material_var_flag_flat                     = ( 1 << 12 ),
	material_var_flag_nocull                   = ( 1 << 13 ),
	material_var_flag_nofog                    = ( 1 << 14 ),
	material_var_flag_ignorez                  = ( 1 << 15 ),
	material_var_flag_decal                    = ( 1 << 16 ),
	material_var_flag_envmapsphere             = ( 1 << 17 ),
	material_var_flag_aoprepass                = ( 1 << 18 ),
	material_var_flag_envmapcameraspace        = ( 1 << 19 ),
	material_var_flag_basealphaenvmapmask      = ( 1 << 20 ),
	material_var_flag_translucent              = ( 1 << 21 ),
	material_var_flag_normalmapalphaenvmapmask = ( 1 << 22 ),
	material_var_flag_needs_software_skinning  = ( 1 << 23 ),
	material_var_flag_opaquetexture            = ( 1 << 24 ),
	material_var_flag_multiply                 = ( 1 << 25 ),
	material_var_flag_suppress_decals          = ( 1 << 26 ),
	material_var_flag_halflambert              = ( 1 << 27 ),
	material_var_flag_wireframe                = ( 1 << 28 ),
	material_var_flag_allowalphatocoverage     = ( 1 << 29 ),
	material_var_flag_alpha_modified_by_proxy  = ( 1 << 30 ),
	material_var_flag_vertexfog                = ( 1 << 31 )
};

enum e_preview_image_ret_val : int {
	material_preview_image_bad = 0,
	material_preview_image_ok,
	material_no_preview_image,
};

enum e_image_format {
	image_format_unknown  = -1,
	image_format_rgba8888 = 0,
	image_format_abgr8888,
	image_format_rgb888,
	image_format_bgr888,
	image_format_rgb565,
	image_format_i8,
	image_format_ia88,
	image_format_p8,
	image_format_a8,
	image_format_rgb888_bluescreen,
	image_format_bgr888_bluescreen,
	image_format_argb8888,
	image_format_bgra8888,
	image_format_dxt1,
	image_format_dxt3,
	image_format_dxt5,
	image_format_bgrx8888,
	image_format_bgr565,
	image_format_bgrx5551,
	image_format_bgra4444,
	image_format_dxt1_onebitalpha,
	image_format_bgra5551,
	image_format_uv88,
	image_format_uvwq8888,
	image_format_rgba16161616f,
	image_format_rgba16161616,
	image_format_uvlx8888,
	image_format_r32f,       // single-channel 32-bit floating point
	image_format_rgb323232f, // note: d3d9 does not have this format
	image_format_rgba32323232f,
	image_format_rg1616f,
	image_format_rg3232f,
	image_format_rgbx8888,

	image_format_null, // dummy format which takes no video memory

	// compressed normal map formats
	image_format_ati2n, // one-surface ati2n / dxn format
	image_format_ati1n, // two-surface ati1n format

	image_format_rgba1010102, // 10 bit-per component render targets
	image_format_bgra1010102,
	image_format_r16f, // 16 bit fp format

	// depth-stencil texture formats
	image_format_d16,
	image_format_d15s1,
	image_format_d32,
	image_format_d24s8,
	image_format_linear_d24s8,
	image_format_d24x8,
	image_format_d24x4s4,
	image_format_d24fs8,
	image_format_d16_shadow,   // specific formats for shadow mapping
	image_format_d24x8_shadow, // specific formats for shadow mapping

	// supporting these specific formats as non-tiled for procedural cpu access (360-specific)
	image_format_linear_bgrx8888,
	image_format_linear_rgba8888,
	image_format_linear_abgr8888,
	image_format_linear_argb8888,
	image_format_linear_bgra8888,
	image_format_linear_rgb888,
	image_format_linear_bgr888,
	image_format_linear_bgrx5551,
	image_format_linear_i8,
	image_format_linear_rgba16161616,

	image_format_le_bgrx8888,
	image_format_le_bgra8888,

	num_image_formats
};

class c_material_var
{
private:
	enum e_indexes {
		_set_vector = 11
	};

public:
	void set_vector( float x, float y, float z )
	{
		using fn = void( __thiscall* )( c_material_var*, float, float, float );
		return ( *( fn** )this )[ this->e_indexes::_set_vector ]( this, x, y, z );
	}
};

class c_material
{
public:
	virtual const char* get_name( ) const                                                                                                    = 0;
	virtual const char* get_texture_group_name( ) const                                                                                      = 0;
	virtual e_preview_image_ret_val get_preview_image_properties( int* i_width, int* i_height, e_image_format* p_image_format,
	                                                              bool* b_translucent ) const                                                = 0;
	virtual e_preview_image_ret_val get_preview_image( unsigned char* p_data, int i_width, int i_height, e_image_format image_format ) const = 0;
	virtual int get_mapping_width( )                                                                                                         = 0;
	virtual int get_mapping_height( )                                                                                                        = 0;
	virtual int get_num_animation_frames( )                                                                                                  = 0;
	virtual bool in_material_page( )                                                                                                         = 0;
	virtual void get_material_offset( float* fl_offset )                                                                                     = 0;
	virtual void get_material_scale( float* fl_scale )                                                                                       = 0;
	virtual c_material* get_material_page( )                                                                                                 = 0;
	virtual c_material_var* find_var( const char* sz_name, bool* b_found, bool b_complain = true )                                           = 0;
	virtual void increment_reference_count( )                                                                                                = 0;
	virtual void decrement_reference_count( )                                                                                                = 0;

	inline void add_reference( )
	{
		increment_reference_count( );
	}

	inline void release( )
	{
		decrement_reference_count( );
	}

	virtual int get_enumeration_id( ) const                                                            = 0;
	virtual void get_low_res_color_sample( float s, float t, float* arr_color ) const                  = 0;
	virtual void recompute_state_snapshots( )                                                          = 0;
	virtual bool is_translucent( )                                                                     = 0;
	virtual bool is_alpha_tested( )                                                                    = 0;
	virtual bool is_vertex_lit( )                                                                      = 0;
	virtual unsigned long long get_vertex_format( ) const                                              = 0;
	virtual bool has_proxy( ) const                                                                    = 0;
	virtual bool uses_env_cubemap( )                                                                   = 0;
	virtual bool needs_tangent_space( )                                                                = 0;
	virtual bool needs_power_of_two_frame_buffer_texture( bool b_check_specific_to_this_frame = true ) = 0;
	virtual bool needs_full_frame_buffer_texture( bool b_check_specific_to_this_frame = true )         = 0;
	virtual bool needs_software_skinning( )                                                            = 0;
	virtual void alpha_modulate( float fl_alpha )                                                      = 0;
	virtual void color_modulate( float r, float g, float b )                                           = 0;
	virtual void set_material_var_flag( e_material_var_flags flag, bool b_enable )                     = 0;
	virtual bool get_material_var_flag( e_material_var_flags flag )                                    = 0;
	virtual void get_reflectivity( c_vector& vec_reflect )                                             = 0;
	virtual bool get_property_flag( e_material_var_flags type )                                        = 0;
	virtual bool is_two_sided( )                                                                       = 0;
	virtual void set_shader( const char* sz_shader_name )                                              = 0;
	virtual int get_num_passes( )                                                                      = 0;
	virtual int get_texture_memory_bytes( )                                                            = 0;
	virtual void refresh( )                                                                            = 0;
	virtual bool needs_lightmap_blend_alpha( )                                                         = 0;
	virtual bool needs_software_lighting( )                                                            = 0;
	virtual int shader_param_count( ) const                                                            = 0;
	virtual c_material_var** get_shader_params( )                                                      = 0;
	virtual bool is_error_material( ) const                                                            = 0;
	virtual void set_use_fixed_function_baked_lighting( bool b_enable )                                = 0;
	virtual float get_alpha_modulation( )                                                              = 0;
	virtual void get_color_modulation( float* r, float* g, float* b )                                  = 0;
	virtual bool is_translucent_under_modulation( float fl_alpha_modulation = 1.0f ) const             = 0;
	virtual c_material_var* find_var_fast( char const* sz_name, unsigned int* pu_token )               = 0;
	virtual void set_shader_and_params( void* p_key_values )                                           = 0;
	virtual const char* get_shader_name( ) const                                                       = 0;
	virtual void delete_if_unreferenced( )                                                             = 0;
	virtual bool is_sprite_card( )                                                                     = 0;
	virtual void call_bind_proxy( void* p_proxy_data )                                                 = 0;
	virtual void refresh_preserving_material_vars( )                                                   = 0;
	virtual bool was_reloaded_from_whitelist( )                                                        = 0;
	virtual bool set_temp_excluded( bool b_set, int n_excluded_dimension_limit )                       = 0;
	virtual int get_reference_count( ) const                                                           = 0;
};

enum e_material_override_type : int {
	material_override_type_normal = 0,
	material_override_type_build_shadows,
	material_override_type_depth_write,
	material_override_type_ssao_depth_write
};

class c_model_render
{
private:
	enum e_indexes {
		_forced_material_override    = 1,
		_is_forced_material_override = 2
	};

public:
	void forced_material_override( c_material* material,
	                               e_material_override_type material_override_type = e_material_override_type::material_override_type_normal,
	                               int override_amount                             = 0 )
	{
		using fn = void( __thiscall* )( c_model_render*, c_material*, e_material_override_type, int );
		return ( *( fn** )this )[ this->e_indexes::_forced_material_override ]( this, material, material_override_type, override_amount );
	}

	bool is_forced_material_override( )
	{
		using fn = bool( __thiscall* )( c_model_render* );
		return ( *( fn** )this )[ this->e_indexes::_is_forced_material_override ]( this );
	}
};