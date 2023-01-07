#pragma once
#include <iostream>

namespace sdk
{
	struct i_net_msg {
		virtual ~i_net_msg( )                             = default;
		virtual void set_net_channel( void* net_channel ) = 0;
		virtual void set_reliable( bool state )           = 0;
		virtual bool process( )                           = 0;
		virtual bool read_from_buffer( void* buffer )     = 0;
		virtual bool write_to_buffer( void* buffer )      = 0;
		virtual bool is_reliable( ) const                 = 0;
		virtual int get_type( ) const                     = 0;
		virtual int get_group( ) const                    = 0;
		virtual const char* get_name( ) const             = 0;
		virtual void* get_net_channel( ) const            = 0;
		virtual const char* to_string( ) const            = 0;
		virtual std::uint32_t get_size( ) const           = 0;
	};
} // namespace sdk
