#pragma once

class c_user_cmd;
class c_angle;

namespace n_movement
{

	struct impl_t {
		void on_create_move_pre( );
		void on_create_move_post( int pre_prediction_flags );

		struct pixel_surf_data_t {
			void reset( )
			{
				this->m_predicted_succesful = false;
				this->m_in_pixel_surf       = false;
				this->m_should_duck         = false;
				this->m_prediction_ticks    = 0;
			}

			bool m_predicted_succesful = false, m_in_pixel_surf = false, m_should_duck = false;
			int m_prediction_ticks      = 0;
			c_user_cmd* m_simulated_cmd = { };
		} m_pixel_surf_data;

	private:
		void bunny_hop( );

		void edge_jump( int pre_prediction_flags );
		void long_jump( );
		void mini_jump( int pre_prediction_flags );
		void jump_bug( int pre_prediction_flags );
		void pixel_surf( const float gravity );

		void rotate_movement( c_user_cmd* cmd, const c_angle& angle );
		void auto_align( c_user_cmd* cmd );
	};
} // namespace n_movement

inline n_movement::impl_t g_movement{ };