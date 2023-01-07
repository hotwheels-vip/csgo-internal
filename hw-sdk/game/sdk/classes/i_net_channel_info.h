#pragma once

#include "../enums/flow.h"

namespace sdk
{
	class i_net_channel_info
	{
	public:
		virtual const char* get_name( void ) const                                                                        = 0;
		virtual const char* get_address( void ) const                                                                     = 0;
		virtual float get_time( void ) const                                                                              = 0;
		virtual float get_time_connected( void ) const                                                                    = 0;
		virtual int get_buffer_size( void ) const                                                                         = 0;
		virtual int get_data_rate( void ) const                                                                           = 0;
		virtual bool is_loop_back( void ) const                                                                           = 0;
		virtual bool is_timing_out( void ) const                                                                          = 0;
		virtual bool is_play_back( void ) const                                                                           = 0;
		virtual float get_latency( sdk::flow flow ) const                                                                 = 0;
		virtual float get_average_latency( sdk::flow flow ) const                                                         = 0;
		virtual float get_average_loss( sdk::flow flow ) const                                                            = 0;
		virtual float get_average_choke( sdk::flow flow ) const                                                           = 0;
		virtual float get_average_data( sdk::flow flow ) const                                                            = 0;
		virtual float get_average_packets( sdk::flow flow ) const                                                         = 0;
		virtual int get_total_data( sdk::flow flow ) const                                                                = 0;
		virtual int get_sequence_number( sdk::flow flow ) const                                                           = 0;
		virtual bool is_valid_packet( sdk::flow flow, int frame_number ) const                                            = 0;
		virtual float get_packet_time( sdk::flow flow, int frame_number ) const                                           = 0;
		virtual int get_packet_bytes( sdk::flow flow, int frame_number, int group ) const                                 = 0;
		virtual bool get_stream_progress( sdk::flow flow, int* received, int* total ) const                               = 0;
		virtual float get_since_last_time_recieved( void ) const                                                          = 0;
		virtual float get_command_interpolation_ammount( sdk::flow flow, int frame_number ) const                         = 0;
		virtual void get_packet_response_latency( sdk::flow flow, int frame_number, int* latencymsecs, int* choke ) const = 0;
		virtual void get_remote_framerate( float* frame_time, float* frame_std_dev ) const                                = 0;
		virtual float get_timeout_seconds( ) const                                                                        = 0;
	};
} // namespace sdk
