#pragma once

class c_base_entity;

class c_client_unknown
{
	enum e_indexes {
		_get_base_entity = 7,
	};

public:
	c_base_entity* get_base_entity( )
	{
		using fn = c_base_entity*( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_get_base_entity ]( this );
	}
};
