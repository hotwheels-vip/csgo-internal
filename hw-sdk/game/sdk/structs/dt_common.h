#pragma once
#include "../../../utils/math/types/vector.h"
#include "../enums/prop_type.h"

namespace sdk
{
	struct d_variant {
		union {
			float f32;
			std::int32_t i32;
			char* str;
			void* data;
			math::vec3 v3f;
			std::int64_t i64;
		};

		sdk::prop_type m_type;
	};
} // namespace sdk
