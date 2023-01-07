#pragma once
#include "../../../utils/vfunc/vfunc.h"
#include "../structs/i_material.h"
#include "../structs/i_texture.h"
#include "i_mat_render_ctx.h"

namespace sdk
{
	class i_material_system
	{
	public:
		void begin_render_target_allocation( )
		{
			virtual_func::call< void >( this, 94 );
		}

		void end_render_target_allocation( )
		{
			virtual_func::call< void >( this, 95 );
		}

		sdk::i_mat_render_ctx* get_render_context( )
		{
			return virtual_func::call< sdk::i_mat_render_ctx* >( this, 115 );
		}

		sdk::i_texture* find_texture( const char* name, const char* group_name, bool complain = true, int unk = 1 )
		{
			return virtual_func::call< sdk::i_texture*, const char*, const char*, bool, int >( this, 91, name, group_name, complain, unk );
		}

		sdk::i_material* find_material( const char* name, const char* group_name = nullptr, bool complain = true,
		                                const char* complain_prefix = nullptr )
		{
			return virtual_func::call< sdk::i_material*, const char*, const char*, bool, const char* >( this, 84, name, group_name, complain,
			                                                                                            complain_prefix );
		}
	};
} // namespace sdk
