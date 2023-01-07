#pragma once

#include "../structs/i_material.h"

namespace sdk
{
	struct iv_model_render {
	private:
		virtual void pad( ) = 0;

	public:
		virtual void forced_material_override( i_material* material, unsigned int override_type = 0, unsigned int overrides = 0 ) = 0;
		virtual bool is_forced_material_override( )                                                                               = 0;
	};
} // namespace sdk
