#pragma once
#include "../../../globals/includes/includes.h"
#include "../../../utils/math/types/vector.h"

namespace sdk
{
	struct c_effect {
	public:
		math::vec3& origin( ) { 
			return *reinterpret_cast<math::vec3*>(this + 0x3B8);
		}
	};
} // namespace sdk
