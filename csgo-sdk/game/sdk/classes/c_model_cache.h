#pragma once
#include "../../../utilities/memory/virtual.h"

class c_model_cache
{
public:
	void begin_lock( )
	{
		g_virtual.call< void >( this, 33 );
	}

	void end_lock( )
	{
		g_virtual.call< void >( this, 34 );
	}
};