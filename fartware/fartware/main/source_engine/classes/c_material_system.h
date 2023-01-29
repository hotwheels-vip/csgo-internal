#pragma once
#include "../../utilities/macros.h"
#include "../classes/c_key_values.h"
#include "c_model_render.h"

struct material_render_target_depth_t;
struct rect_t;

class c_vtftexture;
class c_texture;

struct async_texture_context_t {
	c_texture* texture;
	unsigned int internal_flags;
	int desired_temp_dimension_limit;
	int actual_dimension_limit;
	c_vtftexture* vtf_texture;
};

class c_texture_regenerator
{
public:
	virtual void regenerate_texture_bytes( c_texture* texture, c_vtftexture* vtf_texture, rect_t* rect ) = 0;

	virtual void release( ) = 0;

	virtual bool has_preallocated_scratch_texture( ) const
	{
		return false;
	}

	virtual c_vtftexture* get_preallocated_scratch_texture( )
	{
		return NULL;
	}
};

class c_texture
{
public:
	virtual const char* get_name( void ) const                                     = 0;
	virtual int get_mapping_width( ) const                                         = 0;
	virtual int get_mapping_height( ) const                                        = 0;
	virtual int get_actual_width( ) const                                          = 0;
	virtual int get_actual_height( ) const                                         = 0;
	virtual int get_num_animation_frames( ) const                                  = 0;
	virtual bool is_translucent( ) const                                           = 0;
	virtual bool is_mipmapped( ) const                                             = 0;
	virtual void get_low_res_color_sample( float s, float t, float* color ) const  = 0;
	virtual void* get_resource_data( uint32_t data_type, size_t* num_bytes ) const = 0;
	virtual void increment_reference_count( void )                                 = 0;
	virtual void decrement_reference_count( void )                                 = 0;
	inline void add_ref( )
	{
		increment_reference_count( );
	}
	inline void release( )
	{
		decrement_reference_count( );
	}
	virtual void set_texture_regenerator( c_texture_regenerator* texture_regen, bool release_existing = true )                            = 0;
	virtual void download( rect_t* rect = 0, int additional_creation_flags = 0 )                                                          = 0;
	virtual int get_approximate_vid_bytes( void ) const                                                                                   = 0;
	virtual bool is_error( ) const                                                                                                        = 0;
	virtual bool is_volume_texture( ) const                                                                                               = 0;
	virtual int get_mapping_depth( ) const                                                                                                = 0;
	virtual int get_actual_depth( ) const                                                                                                 = 0;
	virtual void* get_image_format( ) const                                                                                               = 0;
	virtual bool is_render_target( ) const                                                                                                = 0;
	virtual bool is_cube_map( ) const                                                                                                     = 0;
	virtual bool is_normal_map( ) const                                                                                                   = 0;
	virtual bool is_procedural( ) const                                                                                                   = 0;
	virtual bool is_default_pool( ) const                                                                                                 = 0;
	virtual void delete_if_unreferenced( )                                                                                                = 0;
	virtual void swap_contents( c_texture* other )                                                                                        = 0;
	virtual unsigned int get_flags( void ) const                                                                                          = 0;
	virtual void force_lod_override( int num_lods_override_up_or_down )                                                                   = 0;
	virtual void force_exclude_override( int exclude_override )                                                                           = 0;
	virtual void add_downsized_sub_target( const char* name, int downsize_pow2, material_render_target_depth_t depth )                    = 0;
	virtual void set_actuve_sub_target( const char* name )                                                                                = 0;
	virtual int get_reference_count( ) const                                                                                              = 0;
	virtual bool is_temp_excluded( ) const                                                                                                = 0;
	virtual bool can_be_temp_excluded( ) const                                                                                            = 0;
	virtual bool finish_async_download( async_texture_context_t* context, void* data, int num_read_bytes, bool abort, float max_time_ms ) = 0;
	virtual bool is_force_excluded( ) const                                                                                               = 0;
	virtual bool clear_force_exclusion( )                                                                                                 = 0;
};

class c_material_render_context
{
private:
	enum e_indexes {
		_get_render_target_dimensions     = 8,
		_set_frame_buffer_copy_texture    = 20,
		_get_view_port                    = 41,
		_draw_screen_space_rectangle      = 114,
		_copy_render_target_to_texture_ex = 122
	};

public:
	void get_render_target_dimensions( int* width, int* height )
	{
		using fn = void( __thiscall* )( c_material_render_context*, int*, int* );
		return ( *( fn** )this )[ this->e_indexes::_get_render_target_dimensions ]( this, width, height );
	}

	void draw_screen_space_rectangle( c_material* material, int dest_x, int dest_y, int width, int height, float texture_x0, float texture_y0,
	                                  float texture_x1, float texture_y1, int texture_width, int texture_height, void* client_renderable = nullptr,
	                                  int x_dice = 1, int y_dice = 1 )
	{
		using fn =
			void( __thiscall* )( c_material_render_context*, c_material*, int, int, int, int, float, float, float, float, int, int, void*, int, int );
		return ( *( fn** )this )[ this->e_indexes::_draw_screen_space_rectangle ]( this, material, dest_x, dest_y, width, height, texture_x0,
		                                                                           texture_y0, texture_x1, texture_y1, texture_width, texture_height,
		                                                                           client_renderable, x_dice, y_dice );
	}

	void get_view_port( int* x, int* y, int* width, int* height )
	{
		using fn = void( __thiscall* )( c_material_render_context*, int*, int*, int*, int* );
		return ( *( fn** )this )[ this->e_indexes::_get_view_port ]( this, x, y, width, height );
	}

	void copy_render_target_to_texture_ex( c_texture* texture, int unk, rect_t* dst, rect_t* src )
	{
		using fn = void( __thiscall* )( c_material_render_context*, c_texture*, int, rect_t*, rect_t* );
		return ( *( fn** )this )[ this->e_indexes::_copy_render_target_to_texture_ex ]( this, texture, unk, dst, src );
	}

	void set_frame_buffer_copy_texture( c_texture* texture, int texture_index )
	{
		using fn = void( __thiscall* )( c_material_render_context*, c_texture*, int );
		return ( *( fn** )this )[ this->e_indexes::_set_frame_buffer_copy_texture ]( this, texture, texture_index );
	}
};

class c_material_system
{
private:
	enum e_indexes {
		_create_material    = 83,
		_find_material      = 84,
		_find_texture       = 91,
		_get_render_context = 115
	};

public:
	c_material* find_material( char const* material_name, const char* texture_group_name = TEXTURE_GROUP_MODEL, bool complain = true,
	                           const char* complain_prefix = nullptr )
	{
		using fn = c_material*( __thiscall* )( c_material_system*, char const*, char const*, bool, char const* );
		return ( *( fn** )this )[ this->e_indexes::_find_material ]( this, material_name, texture_group_name, complain, complain_prefix );
	}

	c_material* create_material( const char* name, c_key_values* key_values )
	{
		using fn = c_material*( __thiscall* )( c_material_system*, char const*, c_key_values* );
		return ( *( fn** )this )[ this->e_indexes::_create_material ]( this, name, key_values );
	}

	c_texture* find_texture( char const* name, char const* texture_group_name, bool complain = true, int additional_flags = 0 )
	{
		using fn = c_texture*( __thiscall* )( c_material_system*, char const*, char const*, bool, int );
		return ( *( fn** )this )[ this->e_indexes::_find_texture ]( this, name, texture_group_name, complain, additional_flags );
	}

	c_material_render_context* get_render_context( )
	{
		using fn = c_material_render_context*( __thiscall* )( c_material_system* );
		return ( *( fn** )this )[ this->e_indexes::_get_render_context ]( this );
	}
};