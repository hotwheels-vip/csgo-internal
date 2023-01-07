#pragma once
#include "../../../globals/macros/macros.h"

namespace sdk
{
	struct c_event_info {
	public:
		enum
		{
			EVENT_INDEX_BITS    = 8,
			EVENT_DATA_LEN_BITS = 11,
			MAX_EVENT_DATA      = 192,
		};

		short class_id;
		PAD( 0x2 );
		float fire_delay;
		const void* send_table;
		const void* client_class;
		int packed;
		int flags;
		int filter[ 8 ];
		c_event_info* next;
	};
} // namespace sdk
