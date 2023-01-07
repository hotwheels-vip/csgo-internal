#pragma once

class c_mdl_cache
{
public:
	void begin_lock( )
	{
		using fn = void( __thiscall* )( c_mdl_cache* );
		return ( *( fn** )this )[ 33 ]( this );
	}

	void end_lock( )
	{
		using fn = void( __thiscall* )( c_mdl_cache* );
		return ( *( fn** )this )[ 34 ]( this );
	}
};