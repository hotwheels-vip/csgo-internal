#pragma once
#include "../classes/c_key_values.h"
#include "../../utilities/macros.h"
#include "c_model_render.h"

class c_material_system
{
private:
	enum e_indexes {
		_create_material = 83,
		_find_material   = 84,
	};

public:
	c_material* find_material( char const* material_name, const char* texture_group_name = TEXTURE_GROUP_MODEL, bool complain = true,
	                           const char* complain_prefix = nullptr )
	{
		using fn = c_material*( __thiscall* )( c_material_system*, char const*, char const*, bool, char const* );
		return ( *( fn** )this )[ this->e_indexes::_find_material ]( this, material_name, texture_group_name, complain, complain_prefix );
	}

	c_material* create_material( const char* name, c_key_values* key_values )
	{
		using fn = c_material*( __thiscall* )( c_material_system*, char const*, c_key_values* );
		return ( *( fn** )this )[ this->e_indexes::_create_material ]( this, name, key_values );
	}
};