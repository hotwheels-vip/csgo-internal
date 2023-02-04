#pragma once
#include "../../../utilities/memory/virtual.h"

struct texture_t;

class c_texture
{
private:
	unsigned char pad0[ 0x50 ];

public:
	texture_t** m_texture_handles;

	int get_actual_width( )
	{
		return g_virtual.call< int >( this, 3 );
	}

	int get_actual_height( )
	{
		return g_virtual.call< int >( this, 4 );
	}

	void increment_reference_count( )
	{
		g_virtual.call< void >( this, 10 );
	}

	void decrement_reference_count( )
	{
		g_virtual.call< void >( this, 11 );
	}
};