#pragma once
#include "../../../globals/macros/macros.h"
#include "../../../utils/math/types/vector.h"
#include <cstdint>

namespace sdk
{
	class c_view_setup
	{
	public:
		int x, x_old, y, y_old, w, w_old, h, h_old;

		bool ortho;
		float ortho_left;
		float ortho_top;
		float ortho_right;
		float ortho_bottom;
		PAD( 0x7C );
		float fov;
		float fov_viewmodel;
		math::vec3 origin;
		math::vec3 angles;
		float z_near;
		float z_far;
		float z_near_viewmodel;
		float z_far_viewmodel;
		float aspect_ratio;
		float near_blur_depth;
		float near_focus_depth;
		float far_focus_depth;
		float far_blur_depth;
		float near_blur_radius;
		float far_blur_radius;
		int do_f_quality;
		int motion_blur_mode;

		float shutter_time;
		math::vec3 shutter_open_position;
		math::vec3 shutter_open_angles;
		math::vec3 shutter_close_position;
		math::vec3 shutter_close_angles;
		float off_center_top;
		float off_center_bottom;
		float off_center_left;
		float off_center_right;
		bool off_center                           : 1;
		bool render_to_subrect_of_larger_screen   : 1;
		bool do_bloom_and_tone_mapping            : 1;
		bool do_depth_of_field                    : 1;
		bool hdr_target                           : 1;
		bool draw_world_normal                    : 1;
		bool cull_front_faces                     : 1;
		bool cache_full_scene_state               : 1;
		bool render_flashlight_depth_translucents : 1;
		PAD( 0x40 );
	};
} // namespace sdk
