#pragma once
#include "c_vector.h"

class c_collideable
{
private:
	enum e_indexes {
		_obb_mins = 1,
		_obb_maxs = 2,
	};

public:
	c_vector& obb_mins( )
	{
		using fn = c_vector&( __thiscall* )( c_collideable* );
		return ( *( fn** )this )[ this->e_indexes::_obb_mins ]( this );
	}

	c_vector& obb_maxs( )
	{
		using fn = c_vector&( __thiscall* )( c_collideable* );
		return ( *( fn** )this )[ this->e_indexes::_obb_maxs ]( this );
	}
};
