#pragma once
#include "../../../utils/vfunc/vfunc.h"

namespace sdk
{
	class i_panel
	{
	public:
		void set_input_keyboard_state( unsigned int panel, bool state )
		{
			return virtual_func::call< void, unsigned int, bool >( this, 31, panel, state );
		}
		void set_input_mouse_state( unsigned int panel, bool state )
		{
			return virtual_func::call< void, unsigned int, bool >( this, 32, panel, state );
		}
		const char* panel_name( unsigned int panel )
		{
			return virtual_func::call< const char*, unsigned int >( this, 36, panel );
		}
	};
} // namespace sdk
