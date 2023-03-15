#include "c_angle.h"
#include "c_vector.h"

enum e_motion_blur_mode {
	motion_blur_disable = 1,
	motion_blur_game    = 2,
	motion_blur_sfm     = 3
};

class c_view_setup 
{
public:
	unsigned char _0x0000[ 16 ];  // 0x0000
	int m_width;                    // 0x0010
	int m_width_old;                // 0x0014
	int m_height;                   // 0x0018
	int m_height_old;               // 0x001C
	int m_x;                        // 0x0020
	int m_x_old;                    // 0x0024
	int m_y;                        // 0x0028
	int m_y_old;                    // 0x002C
	unsigned char _0x0030[ 128 ]; // 0x0030
	float m_fov;                    // 0x00B0
	float m_fov_viewmodel;          // 0x00B4
	c_vector m_origin;              // 0x00B8
	c_angle m_angles;
	float m_znear;
	float m_zfar;
	float m_znear_viewmodel;
	float m_zfar_viewmodel;
	float m_aspect_ratio;
	float m_near_blur_depth;
	float m_near_focus_depth;
	float m_far_focus_depth;
	float m_far_blur_depth;
	float m_near_blur_radius;
	float m_far_blur_radius;
	int m_dof_quality;
	e_motion_blur_mode m_motion_blur_mode;
	float m_shutter_time;
	c_vector m_shutter_open_position;
	c_angle m_shutter_open_angles;
	c_vector m_shutter_close_position;
	c_angle m_shutter_close_angles;
	float m_off_center_top;
	float m_off_center_bottom;
	float m_off_center_left;
	float m_off_center_right;
	int m_edge_blur;
};
