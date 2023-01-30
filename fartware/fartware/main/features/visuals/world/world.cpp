#include "world.h"
#include "../../../convars/convars.h"
#include "../../../includes.h"
#include "../../../source_engine/classes/c_view_setup.h"

bool world_t::on_draw_view_models( c_view_setup& setup )
{
	float x = static_cast< float >( setup.x );
	float y = static_cast< float >( setup.y );
	float w = static_cast< float >( setup.width );
	float h = static_cast< float >( setup.height );

	static auto resolve_full_frame_depth = convars.find( fnv1a::hash_const( "mat_resolveFullFrameDepth" ) );

	if ( resolve_full_frame_depth->get_int( ) == 1 )
		resolve_full_frame_depth->set_value( 0 );

	bool blur_forward        = false; // should we blur forward | default: true
	float rotation_intensity = 1.f;   // rotation intensity (flicking) | default: 1.f
	float blur_strength      = 2.f;   // blur strength | default: 1.f
	float falling_min        = 5.f;   // falling blur factor min | default: 10.f
	float falling_max        = 10.f;  // falling blur factor max | default: 20.f
	float falling_intensity  = 0.f;   // falling intensity (blur amount) | default: 1.f
	float roll_intensity     = 0.3f;  // roll intensity (blur amount) | default: 0.3f

	static motion_blur_history_t history = { };

	matrix3x4_t current_basis_vectors;
	mathematics.angle_matrix( setup.angles, current_basis_vectors );

	c_vector current_side_vec( current_basis_vectors[ 0 ][ 1 ], current_basis_vectors[ 1 ][ 1 ], current_basis_vectors[ 2 ][ 1 ] );
	c_vector current_forward_vec( current_basis_vectors[ 0 ][ 0 ], current_basis_vectors[ 1 ][ 0 ], current_basis_vectors[ 2 ][ 0 ] );
	c_angle normalized_angles = setup.angles.normalize( );
	c_vector position_change  = history.m_previous_position - setup.origin;

	if ( true ) {
		float horizontal_fov = setup.fov;
		float vertical_fov   = ( setup.aspect_ratio <= 0.0f ) ? ( setup.fov ) : ( setup.fov / setup.aspect_ratio );

		float view_dot_motion = current_forward_vec.dot_product( position_change );
		if ( blur_forward )
			motion_blur_data.m_values[ 2 ] = view_dot_motion;
		else
			motion_blur_data.m_values[ 2 ] = view_dot_motion * fabs( current_forward_vec[ 2 ] );

		float yaw_diff_original = history.m_previous_yaw - normalized_angles.m_y;
		if ( ( ( history.m_previous_yaw - normalized_angles.m_y > 180.0f ) || ( history.m_previous_yaw - normalized_angles.m_y < -180.0f ) ) &&
		     ( ( history.m_previous_yaw + normalized_angles.m_y > -180.0f ) && ( history.m_previous_yaw + normalized_angles.m_y < 180.0f ) ) )
			yaw_diff_original = history.m_previous_yaw + normalized_angles.m_y;

		float side_dot_motion   = current_side_vec.dot_product( position_change );
		float yaw_diff_adjusted = yaw_diff_original + ( side_dot_motion / 3.0f );

		if ( yaw_diff_original < 0.0f )
			yaw_diff_adjusted = std::clamp( yaw_diff_adjusted, yaw_diff_original, 0.0f );
		else
			yaw_diff_adjusted = std::clamp( yaw_diff_adjusted, 0.0f, yaw_diff_original );

		float undampened_yaw           = yaw_diff_adjusted / horizontal_fov;
		motion_blur_data.m_values[ 0 ] = undampened_yaw * ( 1.0f - ( fabs( normalized_angles.m_x ) / 90.0f ) );

		float pitch_compensate_mask = 1.0f - ( ( 1.0f - fabs( current_forward_vec[ 2 ] ) ) * ( 1.0f - fabs( current_forward_vec[ 2 ] ) ) );
		float pitch_diff_original   = history.m_previous_pitch - normalized_angles.m_x;
		float pitch_diff_adjusted   = pitch_diff_original;

		if ( normalized_angles.m_x > 0.0f )
			pitch_diff_adjusted = pitch_diff_original - ( ( view_dot_motion / 2.0f ) * pitch_compensate_mask );
		else
			pitch_diff_adjusted = pitch_diff_original + ( ( view_dot_motion / 2.0f ) * pitch_compensate_mask );

		if ( pitch_diff_original < 0.0f )
			pitch_diff_adjusted = std::clamp( pitch_diff_adjusted, pitch_diff_original, 0.0f );
		else
			pitch_diff_adjusted = std::clamp( pitch_diff_adjusted, 0.0f, pitch_diff_original );

		motion_blur_data.m_values[ 1 ] = pitch_diff_adjusted / vertical_fov;

		motion_blur_data.m_values[ 3 ] = undampened_yaw;
		motion_blur_data.m_values[ 3 ] *= ( std::fabs( normalized_angles.m_x ) / 90.0f ) * ( std::fabs( normalized_angles.m_x ) / 90.0f ) *
		                                  ( std::fabs( normalized_angles.m_x ) / 90.0f );

		if ( ( memory.m_globals->m_real_time - history.m_last_time_update ) > 0.0f )
			motion_blur_data.m_values[ 2 ] /= ( memory.m_globals->m_real_time - history.m_last_time_update ) * 30.0f;
		else
			motion_blur_data.m_values[ 2 ] = 0.0f;

		motion_blur_data.m_values[ 2 ] =
			std::clamp( ( std::fabs( motion_blur_data.m_values[ 2 ] ) - falling_min ) / ( falling_max - falling_min ), 0.0f, 1.0f ) *
			( motion_blur_data.m_values[ 2 ] >= 0.0f ? 1.0f : -1.0f );
		motion_blur_data.m_values[ 2 ] /= 30.0f;

		motion_blur_data.m_values[ 0 ] *= rotation_intensity * blur_strength;
		motion_blur_data.m_values[ 1 ] *= rotation_intensity * blur_strength;
		motion_blur_data.m_values[ 2 ] *= falling_intensity * blur_strength;
		motion_blur_data.m_values[ 3 ] *= roll_intensity * blur_strength;

		if ( memory.m_globals->m_real_time < history.m_no_rotational_motion_blur_until ) {
			motion_blur_data.m_values[ 0 ] = 0.0f;
			motion_blur_data.m_values[ 1 ] = 0.0f;
			motion_blur_data.m_values[ 3 ] = 0.0f;
		} else
			history.m_no_rotational_motion_blur_until = 0.0f;

		history.m_previous_position            = setup.origin;
		history.m_previous_frame_basis_vectors = current_basis_vectors;
		history.m_previous_pitch               = normalized_angles.m_x;
		history.m_previous_yaw                 = normalized_angles.m_y;
		history.m_last_time_update             = memory.m_globals->m_real_time;
	}

	if ( true ) {
		c_texture* full_frame = interfaces.m_material_system->find_texture( "_rt_FullFrameFB", TEXTURE_GROUP_RENDER_TARGET );
		float src_width       = static_cast< float >( full_frame->get_actual_width( ) );
		float src_height      = static_cast< float >( full_frame->get_actual_height( ) );
		int offset{ };

		offset                                  = ( x > 0 ) ? 1 : 0;
		motion_blur_data.m_viewport_values[ 0 ] = float( x + offset ) / ( src_width - 1 );

		offset                                  = ( x < ( src_width - 1 ) ) ? -1 : 0;
		motion_blur_data.m_viewport_values[ 3 ] = float( x + w + offset ) / ( src_width - 1 );

		offset                                  = ( y > 0 ) ? 1 : 0;
		motion_blur_data.m_viewport_values[ 1 ] = float( y + offset ) / ( src_height - 1 );

		offset                                  = ( y < ( src_height - 1 ) ) ? -1 : 0;
		motion_blur_data.m_viewport_values[ 2 ] = float( y + h + offset ) / ( src_height - 1 );
	}

	bool performed_motion_blur = false;
	if ( true ) {
		c_material_render_context* render_context = interfaces.m_material_system->get_render_context( );
		c_texture* full_frame                     = interfaces.m_material_system->find_texture( "_rt_FullFrameFB", TEXTURE_GROUP_RENDER_TARGET );

		int src_width  = full_frame->get_actual_width( );
		int src_height = full_frame->get_actual_height( );
		int vport_width, vport_height, dummy;
		render_context->get_view_port( &dummy, &dummy, &vport_width, &vport_height );

		update_screen_effect_texture( 0, int( x ), int( y ), int( w ), int( h ), false );

		c_material* mat_motion_blur = interfaces.m_material_system->find_material( "dev/motion_blur", TEXTURE_GROUP_OTHER, true );

		bool found                 = false;
		c_material_var* blur_param = mat_motion_blur->find_var( "$MotionBlurInternal", &found, false );
		if ( !found )
			return false;

		blur_param->set_vector_component( motion_blur_data.m_values[ 0 ], 0 );
		blur_param->set_vector_component( motion_blur_data.m_values[ 1 ], 1 );
		blur_param->set_vector_component( motion_blur_data.m_values[ 2 ], 2 );
		blur_param->set_vector_component( motion_blur_data.m_values[ 3 ], 3 );

		c_material_var* viewport_blur_param = mat_motion_blur->find_var( "$MotionBlurViewportInternal", &found, false );
		if ( !found )
			return false;

		viewport_blur_param->set_vector_component( motion_blur_data.m_viewport_values[ 0 ], 0 );
		viewport_blur_param->set_vector_component( motion_blur_data.m_viewport_values[ 1 ], 1 );
		viewport_blur_param->set_vector_component( motion_blur_data.m_viewport_values[ 2 ], 2 );
		viewport_blur_param->set_vector_component( motion_blur_data.m_viewport_values[ 3 ], 3 );

		if ( mat_motion_blur && src_width > 0 && src_height > 0 ) {
			render_context->draw_screen_space_rectangle( mat_motion_blur, 0, 0, vport_width, vport_height, x, y, x + w - 1, y + h - 1, src_width,
			                                             src_height );
			performed_motion_blur = true;
		}
	}

	return performed_motion_blur;
}

void world_t::update_screen_effect_texture( int texture_index, int x, int y, int w, int h, bool dest_fullscreen, rect_t* actual_rect )
{
	rect_t src_rect{ x, y, w, h };

	constexpr auto get_full_frame_frame_buffer_texture = []( int texture_index ) {
		static auto get_full_frame_frame_buffer_texture_fn = reinterpret_cast< c_texture*( __thiscall* )( int ) >(
			memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 81 EC ? ? ? ? 56 8B F1 83 3C" ) ) );

		return get_full_frame_frame_buffer_texture_fn( texture_index );
	};

	c_material_render_context* render_context = interfaces.m_material_system->get_render_context( );
	c_texture* texture                        = get_full_frame_frame_buffer_texture( texture_index );
	int src_width, src_height;
	render_context->get_render_target_dimensions( &src_width, &src_height );
	int dest_width  = texture->get_actual_width( );
	int dest_height = texture->get_actual_height( );

	rect_t dest_rect = src_rect;
	if ( !dest_fullscreen && ( src_width > dest_width || src_height > dest_height ) ) {
		int scale_x      = dest_width / src_width;
		int scale_y      = dest_height / src_height;
		dest_rect.x      = src_rect.x * scale_x;
		dest_rect.y      = src_rect.y * scale_y;
		dest_rect.width  = src_rect.width * scale_x;
		dest_rect.height = src_rect.height * scale_y;
		dest_rect.x      = std::clamp( dest_rect.x, 0, dest_width );
		dest_rect.y      = std::clamp( dest_rect.y, 0, dest_height );
		dest_rect.width  = std::clamp( dest_rect.width, 0, dest_width - dest_rect.x );
		dest_rect.height = std::clamp( dest_rect.height, 0, dest_height - dest_rect.y );
	}

	render_context->copy_render_target_to_texture_ex( texture, 0, &src_rect, dest_fullscreen ? NULL : &dest_rect );
	render_context->set_frame_buffer_copy_texture( texture, texture_index );

	if ( actual_rect ) {
		actual_rect->x      = dest_rect.x;
		actual_rect->y      = dest_rect.y;
		actual_rect->width  = dest_rect.width;
		actual_rect->height = dest_rect.height;
	}
}