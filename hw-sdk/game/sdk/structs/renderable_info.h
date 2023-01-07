#pragma once
#include "../classes/i_client_renderable.h"

namespace sdk
{
	struct renderable_info {
		i_client_renderable* renderable;
		void* alpha_property;
		int enum_count;
		int render_frame;
		unsigned short first_shadow;
		unsigned short leaft_list;
		short area;
		std::uint16_t flags;
		std::uint16_t flags_alt;
		math::vec3 bloated_absoulte_mins;
		math::vec3 bloated_absoulte_maxs;
		math::vec3 absoulte_mins;
		math::vec3 absoulte_maxs;
		PAD( 0x4 );
	};
} // namespace sdk
