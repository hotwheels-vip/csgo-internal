#pragma once
#include "../../../utilities/memory/virtual.h"

class c_surface
{
public:
	void unlock_cursor() {
		g_virtual.call< void >( this, 66 );
	}
};