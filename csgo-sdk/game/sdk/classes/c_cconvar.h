#pragma once
#include "../../../utilities/memory/virtual.h"
#include "../../datatypes/fn_change_callback.h"
#include "c_utl_vector.h"

using fn_command_callback_v1_t = void( __cdecl* )( );
class c_cconvar
{
public:
	const char* get_name( )
	{
		return g_virtual.call< const char* >( this, 5 );
	}

	float get_float( ) 
	{
		unsigned int xored = *reinterpret_cast< unsigned int* >( &m_parent->m_float_value ) ^ reinterpret_cast< unsigned int >( this );
		return *reinterpret_cast< float* >( &xored );
	}

	int get_int( ) 
	{
		return static_cast< int >( m_parent->m_int_value ^ reinterpret_cast< int >( this ) );
	}

	bool get_bool( )
	{
		return !!get_int( );
	}

	const char* get_string( ) const
	{
		char const* sz_value = m_parent->m_string;
		return sz_value ? sz_value : "";
	}

	void set_value( const char* value )
	{
		return g_virtual.call< void >( this, 14, value );
	}

	void set_value( float value )
	{
		return g_virtual.call< void >( this, 15, value );
	}

	void set_value( int value )
	{
		return g_virtual.call< void >( this, 16, value );
	}

public:
	unsigned char pad0[ 0x4 ];
	c_cconvar* m_next;
	bool m_registered;
	const char* m_name;
	const char* m_help_string;
	int m_flags;
	fn_command_callback_v1_t m_callback;
	c_cconvar* m_parent;
	const char* sm_default_value;
	char* m_string;
	int m_string_length;
	float m_float_value;
	int m_int_value;
	bool m_bool_has_min;
	float m_float_min_value;
	bool m_bool_has_max;
	float m_float_max_value;
	c_utl_vector< fn_change_callback_t > fn_change_callbacks;
};