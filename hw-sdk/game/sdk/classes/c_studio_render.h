#pragma once
#include "../../../utils/helpers/primitive_definitions/primitives.h"
#include "../structs/color.h"
#include "../structs/i_material.h"
#include "../structs/studio.h"
#include "i_client_renderable.h"

namespace sdk
{
	struct studio_decal_handle_t {
		int unused;
	};

	struct draw_model_info_t {
		sdk::studio_hdr* studio_hdr;
		unk* hardware_data;
		sdk::studio_decal_handle_t decals;
		int mskin;
		int mbody;
		int mhitbox_set;
		sdk::i_client_renderable* renderable;
		int lod;
		unk* color_meshes;
		bool static_lightning;
	};

	class c_studio_render
	{
	public:
		void set_color_modulation( float const* color )
		{
			virtual_func::call< void, float const* >( this, 27, color );
		}

		void set_alpha_modulation( float alpha )
		{
			virtual_func::call< void, float >( this, 28, alpha );
		}

		void forced_material_override( sdk::i_material* material, int type = 0, int overrides = 0 )
		{
			return virtual_func::call< void, sdk::i_material*, int, int >( this, 33, material, type, overrides );
		}

		bool is_forced_material_override( )
		{
			return virtual_func::call< bool >( this, 34 );
		}

		PAD( 0x240 );
		color color_modulation;
		float alpha_modulation;
		sdk::i_material* forced_material[ 4 ];
		int forced_material_type;
		int forced_material_index[ 4 ];
		int forced_material_index_count;
	};
} // namespace sdk
