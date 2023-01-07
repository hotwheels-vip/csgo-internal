#pragma once
#include "i_client_unknown.h"

struct client_think_handle_pointer;

typedef client_think_handle_pointer* client_think_handle;

namespace sdk
{
	struct i_client_thinkable {
	public:
		virtual i_client_unknown* get_i_client_unknown( )                 = 0;
		virtual void client_think( )                                      = 0;
		virtual client_think_handle get_think_handle( )                   = 0;
		virtual void set_think_handle( client_think_handle think_handle ) = 0;
		virtual void release( )                                           = 0;
	};
} // namespace sdk
