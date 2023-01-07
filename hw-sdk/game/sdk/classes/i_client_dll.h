#pragma once

#include "../../../utils/vfunc/vfunc.h"
#include "../structs/i_client_class.h"

namespace sdk
{
	struct i_client_dll {
		sdk::client_class* get_all_classes( )
		{
			return virtual_func::call< sdk::client_class* >( this, 8 );
		};
		sdk::std_recv_proxies* get_std_recv_proxies( )
		{
			return virtual_func::call< sdk::std_recv_proxies* >( this, 48 );
		}
		bool dispatch_user_message( int msg_type, unsigned int arg1, unsigned int length, const void* data = nullptr )
		{
			return virtual_func::call< void*, int, unsigned int, unsigned int, const void* >( this, 38, msg_type, arg1, length, data );
		}
	};
} // namespace sdk
