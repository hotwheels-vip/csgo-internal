#pragma once
#include "../../../utils/math/types/vector.h"
#include "../../../utils/modules/modules.h"
#include "c_usercmd.h"
#include <minwindef.h>

namespace sdk
{
	struct c_base_player;

	struct move_data_t {
	public:
		bool first_run_of_functions : 1;
		bool game_code_moved_player : 1;
		bool no_air_control         : 1;
		DWORD player_handle;
		int impulse_command;
		math::vec3 view_angles;
		math::vec3 abs_view_angles;
		int buttons;
		int old_buttons;
		float forward_move;
		float side_move;
		float up_move;
		float max_speed;
		float client_max_speed;
		math::vec3 velocity;
		math::vec3 trailing_velocity;
		float trailing_velocity_time;
		math::vec3 angles;
		math::vec3 old_angles;
		float out_step_height;
		math::vec3 wish_velocity;
		math::vec3 jump_velocity;
		math::vec3 constraint_center;
		float constraint_radius;
		float constraint_width;
		float constraint_speed_factor;
		bool constraint_past_radius;

	private:
		math::vec3 vec_abs_origin;
	};

	class i_move_helper
	{
	private:
		virtual void unk1( ) = 0;

	public:
		virtual void set_host( sdk::c_base_player* host_player ) = 0;

	private:
		virtual void unk2( ) = 0;
		virtual void unk3( ) = 0;

	public:
		virtual bool process_impacts( ) = 0;
	};

	class i_game_movement
	{
	public:
		virtual ~i_game_movement( ){ };
		virtual void process_movement( sdk::c_base_player* player, void* movement )           = 0;
		virtual void reset( )                                                                 = 0;
		virtual void start_track_prediction_errors( sdk::c_base_player* player )              = 0;
		virtual void finish_track_prediction_errors( sdk::c_base_player* player )             = 0;
		virtual void difference_print( char const* fmt, ... )                                 = 0;
		virtual math::vec3 const& player_mins( bool ducked ) const                            = 0;
		virtual math::vec3 const& player_maxs( bool ducked ) const                            = 0;
		virtual math::vec3 const& player_view_offset( bool ducked ) const                     = 0;
		virtual bool is_moving_player_stuck( ) const                                          = 0;
		virtual sdk::c_base_player* get_moving_player( ) const                                = 0;
		virtual void unblock_pusher( sdk::c_base_player* player, sdk::c_base_player* pusher ) = 0;
		virtual void setup_movement_bounds( move_data_t* movement )                           = 0;
	};

	class i_prediction
	{
	public:
		virtual ~i_prediction( ){ };
		virtual void initialize( void ) = 0;
		virtual void shutdown( void )   = 0;

		virtual void update( int start_frame, bool valid_frame, int incoming_ack, int outgoing_command )               = 0;
		virtual void pre_entity_packet_recieved( int commands_ack, int current_world_update_packet )                   = 0;
		virtual void post_entity_packet_recieved( void )                                                               = 0;
		virtual void post_network_data_recieved( int commands_ack )                                                    = 0;
		virtual void on_recieve_uncompressed_packet( void )                                                            = 0;
		virtual void get_view_origin( math::vec3& origin )                                                             = 0;
		virtual void set_view_origin( math::vec3& origin )                                                             = 0;
		virtual void get_view_angles( math::vec3& angles )                                                             = 0;
		virtual void set_view_angles( math::vec3& angles )                                                             = 0;
		virtual void get_local_view_angles( math::vec3& angles )                                                       = 0;
		virtual void set_local_view_angles( math::vec3& angles )                                                       = 0;
		virtual bool in_prediction( void ) const                                                                       = 0;
		virtual bool is_first_time_predicted( void ) const                                                             = 0;
		virtual int get_last_ack_command_number( void ) const                                                          = 0;
		virtual int get_incoming_packet_number( void ) const                                                           = 0;
		virtual void check_moving_ground( sdk::c_base_player* player, double frame_time )                              = 0;
		virtual void run_command( sdk::c_base_player* player, c_user_cmd* cmd, i_move_helper* move_helper )            = 0;
		virtual void setup_move( sdk::c_base_player* player, c_user_cmd* cmd, i_move_helper* move_helper, void* data ) = 0;
		virtual void finish_move( sdk::c_base_player* player, c_user_cmd* cmd, void* data )                            = 0;

		std::int32_t& predicted_commands( )
		{
			return *reinterpret_cast< std::int32_t* >( reinterpret_cast< std::uintptr_t >( this ) + 0x1C );
		}

		std::int32_t& previous_start_frame( )
		{
			return *reinterpret_cast< std::int32_t* >( reinterpret_cast< std::uintptr_t >( this ) + 0xC );
		}

		std::int32_t& server_commands_acknowledged( )
		{
			return *reinterpret_cast< std::int32_t* >( reinterpret_cast< std::uintptr_t >( this ) + 0x20 );
		}

		void restore_entity_to_predicted_frame( std::int32_t frame )
		{
			static auto pattern = g_client_dll.pattern_scan( _( "55 8B EC 8B 4D ? 56 8B C1 83 C0 ? 8B 75" ) ).as< void( __stdcall* )( int, int ) >( );

			pattern( 0, frame );
		}

		PAD( 8 );
		bool m_in_prediction;
		bool m_old_cl_predict_value;
		bool m_engine_paused;
		PAD( 13 );
		bool m_is_first_time_predicted;
	};
} // namespace sdk
