#pragma once
#include "../../../globals/macros/macros.h"
#include "../../../utils/math/types/matrix.h"
#include "../../../utils/math/types/vector.h"

namespace sdk
{
	struct i_model {
	public:
		void* handle;
		char name[ 260 ];
		int load_flags;
		int server_count;
		int type;
		int flags;
		math::vec2< float > mins;
		math::vec2< float > maxs;
		float radius;
		PAD( 0x1C );
	};
} // namespace sdk
