#pragma once

#include "type_description.h"

namespace sdk
{
	struct data_map {
	public:
		type_description* description;
		int fields;
		const char* class_name;
		data_map* base_map;
		bool chains_validated;
		bool packed_offsets_computed;
		int packed_size;
	};
} // namespace sdk
