#pragma once
#include "../../../utilities/memory/virtual.h"

class c_input_system
{
public:
	void enable_input( bool enable )
	{
		g_virtual.call< void >( this, 11, enable );
	}
};