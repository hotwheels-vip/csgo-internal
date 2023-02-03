#pragma once

class c_base_client;

class c_client_networkable
{
private:
	enum e_indexes {
		_release             = 1,
		_get_client_class    = 2,
		_on_pre_data_changed = 4,
		_on_data_changed     = 5,
		_pre_data_update     = 6,
		_post_data_update    = 7
	};

public:
	void release( )
	{
		using fn = void( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_release ]( this );
	}

	c_base_client* get_client_class( )
	{
		using fn = c_base_client*( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_get_client_class ]( this );
	}

	void on_pre_data_changed( int update_type )
	{
		using fn = void( __thiscall* )( void*, int );
		return ( *( fn** )this )[ this->e_indexes::_on_pre_data_changed ]( this, update_type );
	}

	void on_data_changed( int update_type )
	{
		using fn = void( __thiscall* )( void*, int );
		return ( *( fn** )this )[ this->e_indexes::_on_data_changed ]( this, update_type );
	}

	void pre_data_update( int update_type )
	{
		using fn = void( __thiscall* )( void*, int );
		return ( *( fn** )this )[ this->e_indexes::_pre_data_update ]( this, update_type );
	}

	void post_data_update( int update_type )
	{
		using fn = void( __thiscall* )( void*, int );
		return ( *( fn** )this )[ this->e_indexes::_post_data_update ]( this, update_type );
	}
};
