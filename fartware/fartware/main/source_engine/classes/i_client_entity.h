#pragma once
#include "c_client_networkable.h"
#include "c_client_renderable.h"
#include "c_client_thinkable.h"
#include "c_client_unknown.h"

// easy way to access all base entity related functions
struct i_client_entity : public c_client_unknown, public c_client_renderable, public c_client_networkable, public c_client_thinkable {
	virtual void release( ) = 0;

	virtual c_vector& i_get_abs_origin( ) = 0;
	virtual c_vector& i_get_abs_angles( ) = 0;

	void set_abs_origin( c_vector& origin );

	template< typename T >
	T as( )
	{
		return reinterpret_cast< T >( this );
	}
};
