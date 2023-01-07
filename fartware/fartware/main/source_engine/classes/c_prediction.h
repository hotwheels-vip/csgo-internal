#pragma once
#include "c_angle.h"
#include "c_user_cmd.h"
#include "c_utl_vector.h"
#include "c_vector.h"

class c_move_data
{
public:
	bool m_first_run_of_functions : 1;
	bool m_game_code_moved_player : 1;
	bool m_no_air_control         : 1;
	unsigned int m_player_handle; // edict index on server, client entity handle on client=
	int m_impulse_command;        // impulse command issued.
	c_angle m_view_angles;        // command view angles (local space)
	c_angle m_abs_view_angles;    // command view angles (world space)
	int m_buttons;                // attack buttons.
	int m_old_buttons;            // from host_client->oldbuttons;
	float m_forward_move;
	float m_side_move;
	float m_up_move;
	float m_max_speed;
	float m_client_max_speed;
	c_vector m_velocity; // edict::velocity	// current movement direction.
	c_vector m_trailing_velocity;
	float m_trailing_velocity_time;
	c_vector m_angles; // edict::angles
	c_vector m_old_angles;
	float m_out_step_height; // how much you climbed this move
	c_vector m_out_wish_vel; // this is where you tried
	c_vector m_out_jump_vel; // this is your jump velocity
	c_vector m_constraint_center;
	float m_constraint_radius;
	float m_constraint_width;
	float m_constraint_speed_factor;
	bool m_constraint_past_radius;
	c_vector m_abs_origin;
};

class c_base_entity;
class c_physics_surface_props;
class c_game_trace;
enum e_sound_level;
class c_move_helper
{
public:
	virtual const char* get_name( void* entity ) const                                                                           = 0;
	virtual void set_host( c_base_entity* host )                                                                                 = 0;
	virtual void reset_touch_list( )                                                                                             = 0;
	virtual bool add_to_touched( const c_game_trace& trace, const c_vector& impact_velocity )                                    = 0;
	virtual void process_impacts( )                                                                                              = 0;
	virtual void con_n_printf( int n_index, char const* fmt, ... )                                                               = 0;
	virtual void start_sound( const c_vector& vorigin, int channel, char const* sample, float volume, e_sound_level soundlevel, int f_flags,
	                          int i_pitch )                                                                                      = 0;
	virtual void start_sound( const c_vector& origin, const char* soundname )                                                    = 0;
	virtual void playback_event_full( int flags, int client_index, unsigned short event_index, float delay, c_vector& origin, c_vector& angles,
	                                  float fl_param1, float fl_param2, int i_param1, int i_param2, int b_param1, int b_param2 ) = 0;
	virtual bool player_falling_damage( )                                                                                        = 0;
	virtual void player_set_animation( int player_animation )                                                                    = 0;
	virtual c_physics_surface_props* get_surface_props( )                                                                        = 0;
	virtual bool is_world_entity( const unsigned long& entity )                                                                  = 0;
};

class c_game_movement
{
public:
	virtual ~c_game_movement( ) { }
	virtual void process_movement( c_base_entity* entity, c_move_data* p_move ) = 0;
	virtual void reset( )                                                       = 0;
	virtual void start_track_prediction_errors( c_base_entity* entity )         = 0;
	virtual void finish_track_prediction_errors( c_base_entity* entity )        = 0;
	virtual void diff_print( char const* fmt, ... )                             = 0;
	virtual c_vector const& get_player_mins( bool ducked ) const                = 0;
	virtual c_vector const& get_player_maxs( bool ducked ) const                = 0;
	virtual c_vector const& get_player_view_offset( bool ducked ) const         = 0;
	virtual bool is_moving_player_stuck( ) const                                = 0;
	virtual c_base_entity* get_moving_player( ) const                           = 0;
	virtual void unblock_pusher( c_base_entity* entity, c_base_entity* pusher ) = 0;
	virtual void setup_movement_bounds( c_move_data* move )                     = 0;
};

class c_prediction
{
private:
	enum e_indexes {
		_update                = 3,
		_get_local_view_angles = 12,
		_set_local_view_angles = 13,
		_check_moving_ground   = 18,
		_setup_move            = 20,
		_finish_move           = 21
	};

public:
	char pad0[ 0x4 ];
	unsigned int m_last_ground;
	bool m_in_prediction;
	bool m_is_first_time_predicted;
	bool m_engine_paused;
	bool m_old_cl_predict_value;
	int i_previous_start_frame;
	int n_incoming_packet_number;
	float fl_last_server_world_time_stamp;

	struct split_t {
		bool m_is_first_time_predicted;
		char pad0[ 0x3 ];
		int m_commands_predicted;
		int m_server_commands_acknowledged;
		int m_previous_ack_had_errors;
		float m_ideal_pitch;
		int m_last_command_acknowledged;
		bool m_previous_ack_error_triggers_full_latch_reset;
		c_utl_vector< unsigned int > m_entities_with_prediction_errors_in_last_ack;
		bool m_performed_tick_shift;
	};

	split_t split[ 1 ];

public:
	void update( int start_frame, bool valid_frame, int incoming_acknowledged, int outgoing_command )
	{
		using fn = void( __thiscall* )( c_prediction*, int, bool, int, int );
		return ( *( fn** )this )[ this->e_indexes::_update ]( this, start_frame, valid_frame, incoming_acknowledged, outgoing_command );
	}

	void get_local_view_angles( c_angle& view_angles )
	{
		using fn = void( __thiscall* )( c_prediction*, c_angle& );
		return ( *( fn** )this )[ this->e_indexes::_get_local_view_angles ]( this, std::ref( view_angles ) );
	}

	void set_local_view_angles( c_angle& view_angles )
	{
		using fn = void( __thiscall* )( c_prediction*, c_angle& );
		return ( *( fn** )this )[ this->e_indexes::_set_local_view_angles ]( this, std::ref( view_angles ) );
	}

	void check_moving_ground( c_base_entity* entity, double frametime )
	{
		using fn = void( __thiscall* )( c_prediction*, c_base_entity*, double );
		return ( *( fn** )this )[ this->e_indexes::_check_moving_ground ]( this, entity, frametime );
	}

	void setup_move( c_base_entity* entity, c_user_cmd* cmd, c_move_helper* p_helper, c_move_data* move_data )
	{
		using fn = void( __thiscall* )( c_prediction*, c_base_entity*, c_user_cmd*, c_move_helper*, c_move_data* );
		return ( *( fn** )this )[ this->e_indexes::_setup_move ]( this, entity, cmd, p_helper, move_data );
	}

	void finish_move( c_base_entity* entity, c_user_cmd* cmd, c_move_data* move_data )
	{
		using fn = void( __thiscall* )( c_prediction*, c_base_entity*, c_user_cmd*, c_move_data* );
		return ( *( fn** )this )[ this->e_indexes::_finish_move ]( this, entity, cmd, move_data );
	}
};
