#pragma once
#include "../../../globals/macros/macros.h"
#include "../../../utils/math/types/matrix.h"
#include "../../../utils/math/types/vector.h"

#include "../structs/i_model.h"

struct i_client_renderable;

namespace sdk
{
	struct model_render_info {
	public:
		math::vec3 origin;
		math::vec3 angles;
		PAD( 0x4 );
		i_client_renderable* renderable;
		i_model* model;
		math::matrix_3x4* model_to_world;
		math::matrix_3x4* lighting_offset;
		math::vec3* lighting_origin;
		int flags;
		int entity_index;
		int skin;
		int body;
		int hitbox_set;
		void* instance;
	};
} // namespace sdk
