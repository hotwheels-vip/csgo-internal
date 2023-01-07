#pragma once
#include <iostream>

#include "i_client_unknown.h"

#include "../../../utils/math/types/matrix.h"
#include "../../../utils/math/types/vector.h"
#include "../structs/i_model.h"

typedef std::uintptr_t client_shadow_handle;
typedef std::uintptr_t client_render_handle;
typedef std::uintptr_t model_instance_handle;

struct bf_read;

namespace sdk
{
	struct i_client_renderable {
	public:
		virtual i_client_unknown* get_i_client_unkown( ) = 0;
		virtual math::vec3& get_render_origin( )         = 0;
		virtual math::vec3& get_render_angles( )         = 0;
		virtual bool should_draw( )                      = 0;
		virtual int get_render_flags( )                  = 0;

	private:
		virtual void padding( ) = 0;

	public:
		virtual client_shadow_handle get_shadow_handle( ) = 0;
		virtual client_render_handle get_render_handle( ) = 0;

	private:
		virtual void padding_( ) = 0;

	public:
		virtual int draw_model( int flags, std::uintptr_t& instance )                                                                        = 0;
		virtual int get_body( )                                                                                                              = 0;
		virtual void get_color_modulation( float* color )                                                                                    = 0;
		virtual bool lod_test( )                                                                                                             = 0;
		virtual bool setup_bones( math::matrix_3x4* bone_to_world_out, int max_bones, int bone_mask, float current_time )                    = 0;
		virtual void setup_weights( math::matrix_3x4* bone_to_world, int flex_weight_count, float flex_weights, float flex_delayed_weights ) = 0;
		virtual void do_animation_event( )                                                                                                   = 0;
		virtual std::uintptr_t* get_pvs_norify_interface( )                                                                                  = 0;
		virtual void get_render_bounds( math::vec3& minimum, math::vec3& maximum )                                                           = 0;
		virtual void get_render_bounds_worldspace( math::vec3& mins, math::vec3& maxs )                                                      = 0;
		virtual void get_shadow_render_bounds( math::vec3& mins, math::vec3& maxs, int shadow_type )                                         = 0;
		virtual bool should_receive_projected_textures( int flags )                                                                          = 0;
		virtual bool get_shadow_cast_distance( float* p_dist, int shadow_type ) const                                                        = 0;
		virtual bool get_shadow_cast_direction( math::vec3* p_direction, int shadow_type ) const                                             = 0;
		virtual bool is_shadow_dirty( )                                                                                                      = 0;
		virtual void mark_shadow_dirty( bool b_dirty )                                                                                       = 0;
		virtual i_client_renderable* get_shadow_parent( )                                                                                    = 0;
		virtual i_client_renderable* first_shadow_child( )                                                                                   = 0;
		virtual i_client_renderable* next_shadow_peer( )                                                                                     = 0;
		virtual int shadow_cast_type( )                                                                                                      = 0;
		virtual void create_model_instance( )                                                                                                = 0;
		virtual model_instance_handle get_model_instance( )                                                                                  = 0;
		virtual const math::matrix_3x4& renderable_to_world_transform( )                                                                     = 0;
		virtual int lookup_attachment( const char* p_attachment_name )                                                                       = 0;
		virtual bool get_attachment( int number, math::vec3& origin, math::vec3& angles )                                                    = 0;
		virtual bool get_attachment( int number, math::matrix_3x4& matrix )                                                                  = 0;
		virtual float* get_render_clip_plane( void )                                                                                         = 0;
		virtual int get_skin( )                                                                                                              = 0;
		virtual void on_threaded_draw_setup( )                                                                                               = 0;
		virtual bool uses_flex_delayed_weights( )                                                                                            = 0;
		virtual void record_tool_message( )                                                                                                  = 0;
		virtual bool should_draw_for_split_screen_user( int n_slot )                                                                         = 0;
		virtual std::uint8_t override_alpha_modulation( std::uint8_t n_alpha )                                                               = 0;
		virtual std::uint8_t override_shadow_alpha_modulation( std::uint8_t n_alpha )                                                        = 0;

		i_model* get_model( )
		{
			return virtual_func::call< sdk::i_model* >( this, 8 );
		}

		math::vec3 get_bone_position( int bone_index )
		{
			if ( math::matrix_3x4 matrix[ 128 ]; setup_bones( matrix, 128, 256, 0.f ) ) {
				auto bone_matrix = matrix[ bone_index ];

				return {
					bone_matrix[ 0 ][ 3 ],
					bone_matrix[ 1 ][ 3 ],
					bone_matrix[ 2 ][ 3 ],
				};
			}

			return { };
		}

		math::vec3 get_bone_position( int bone_index, math::matrix_3x4* matrix )
		{
			auto bone_matrix = matrix[ bone_index ];

			return {
				bone_matrix[ 0 ][ 3 ],
				bone_matrix[ 1 ][ 3 ],
				bone_matrix[ 2 ][ 3 ],
			};
		}
	};
} // namespace sdk
