#pragma once
#include "../../../utils/vfunc/vfunc.h"

namespace sdk
{
	class i_mat_render_ctx
	{
	public:
		int release( )
		{
			return virtual_func::call< int >( this, 1 );
		}

		void set_render_target( void* i_texture )
		{
			return virtual_func::call< void >( this, 6 );
		}

		void draw_screen_space_rectangle( void* material, int destx, int desty, int width, int height, float src_texture_x0, float src_texture_y0,
		                                  float src_texture_x1, float src_texture_y1, int src_texture_width, int src_texture_height,
		                                  void* client_renderable = NULL, int x_dice = 1, int y_dice = 1 )
		{
			return virtual_func::call< void, void*, int, int, int, int, float, float, float, float, int, int, void*, int, int >(
				this, 114, material, destx, desty, width, height, src_texture_x0, src_texture_y0, src_texture_x1, src_texture_y1, src_texture_width,
				src_texture_height, client_renderable, x_dice, y_dice );
		}

		void push_render_target_and_viewport( )
		{
			return virtual_func::call< void >( this, 119 );
		}

		void pop_render_target_and_viewport( )
		{
			return virtual_func::call< void >( this, 120 );
		}
	};
} // namespace sdk
