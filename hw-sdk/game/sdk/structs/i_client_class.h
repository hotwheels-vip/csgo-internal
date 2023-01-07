#pragma once

#include "../enums/class_ids.h"
#include "dt_recv.h"

namespace sdk
{
	struct client_class {
		using create_client_class_fn = void*( __cdecl* )( std::int32_t, std::int32_t );
		using create_event_fn        = void*( __cdecl* )( );

		create_client_class_fn create;
		create_event_fn create_event;
		char* network_name;
		sdk::recv_table* recv_table;
		client_class* next;
		sdk::class_ids class_id;
	};
} // namespace sdk
