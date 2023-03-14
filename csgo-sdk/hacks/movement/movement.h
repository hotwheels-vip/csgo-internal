#pragma once

class c_user_cmd;
class c_angle;

enum edgebug_type_t : int {
	eb_standing = 0,
	eb_ducking,
};

namespace n_movement
{
	struct impl_t {
		struct edgebug_data_t {
			edgebug_type_t m_edgebug_method{ };

			bool m_will_edgebug{ };
			bool m_will_fail{ };
			bool m_strafing{ };

			float m_yaw_delta{ };
			float m_starting_yaw{ };

			float m_side_move{ };
			float m_forward_move{ };

			float m_saved_mousedx{ };
			int m_ticks_to_stop{ };
			int m_last_tick{ };

			void reset( );
		} m_edgebug_data;

		struct jumpbug_data_t {
			int m_height_diff                    = 0.f;
			float m_vertical_velocity_at_landing = 0.f;
			float m_abs_height_diff              = 0.f;
			int m_ticks_till_land                = 0;
			bool m_can_jb                        = false;
		} m_jumpbug_data;

		struct pixelsurf_data_t {
			bool m_predicted_succesful = false, m_in_pixel_surf = false, m_should_duck = false;
			int m_prediction_ticks      = 0;
			c_user_cmd* m_simulated_cmd = { };

			void reset( );
		} m_pixelsurf_data;

		struct autoduck_data_t {
			bool m_did_land_ducking  = false;
			bool m_did_land_standing = false;

			float m_ducking_vert  = 0.f;
			float m_standing_vert = 0.f;

			void reset( );
		} m_autoduck_data;
		void on_create_move_pre( );
		void on_create_move_post( );

		void on_frame_stage_notify( int stage );

		void rotate_movement( c_user_cmd* cmd, c_angle& angle );

	private:
		void bunny_hop( );

		void edge_jump( );

		void edge_bug( );

		void long_jump( );

		void mini_jump( );

		void jump_bug( );

		void auto_duck( );

		void pixel_surf_locking( float target_ps_velocity );

		void pixel_surf_fix( );

		void pixel_surf( float target_ps_velocity );

		void detect_edgebug( c_user_cmd* cmd );

		void auto_align( c_user_cmd* cmd );

		void strafe_to_yaw( c_user_cmd* cmd, c_angle& angle, const float yaw );

		void movement_fix( const c_angle& old_view_point );
	};
} // namespace n_movement

inline n_movement::impl_t g_movement{ };
