#pragma once
#include "../../../globals/includes/includes.h"

#include "../enums/field_type.h"
#include "../enums/td_offset.h"

namespace sdk
{
	struct data_map;

	struct type_description {
	public:
		field_type type;
		const char* name;
		int offset[ static_cast< unsigned int >( td_offset::COUNT ) ]; // thanx c++
		unsigned short size;
		short flags;
		PAD( 0xC );
		data_map* type_description;
		PAD( 0x18 );
	};
} // namespace sdk
