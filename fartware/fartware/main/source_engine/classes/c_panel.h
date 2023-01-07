#pragma once

class c_panel
{
	enum indexes {
		_get_panel_name = 36
	};

public:
	const char* get_panel_name( int index )
	{
		using fn = const char*( __thiscall* )( c_panel*, int );
		return ( *( fn** )this )[ this->indexes::_get_panel_name ]( this, index );
	}
};