#pragma once
#include "../../utilities/macros.h"
#include "c_model_render.h"

class c_material_system
{
private:
	enum e_indexes {
		_find_material = 84,
	};

public:
	c_material* find_material( char const* material_name, const char* texture_group_name = TEXTURE_GROUP_MODEL, bool complain = true,
	                           const char* complain_prefix = nullptr )
	{
		using fn = c_material*( __thiscall* )( c_material_system*, char const*, char const*, bool, char const* );
		return ( *( fn** )this )[ this->e_indexes::_find_material ]( this, material_name, texture_group_name, complain, complain_prefix );
	}
};