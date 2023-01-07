#pragma once
#include "../../../utils/vfunc/vfunc.h"

namespace sdk
{
	class i_model_cache
	{
	public:
		void begin_lock( )
		{
			return virtual_func::call< void >( this, 33 );
		}
		void end_lock( )
		{
			return virtual_func::call< void >( this, 34 );
		}
	};
} // namespace sdk
