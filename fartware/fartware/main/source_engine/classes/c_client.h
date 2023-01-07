#pragma once

class c_client_networkable;
using create_client_class_fn = c_client_networkable*( __cdecl* )( int, int );
using create_event_fn        = c_client_networkable*( __cdecl* )( );

class c_standart_recv_proxies;
struct recv_table_t;
enum class e_class_index;

class c_base_client
{
public:
	create_client_class_fn m_create_fn;
	create_event_fn m_create_event_fn;
	char* m_network_name;
	recv_table_t* m_recv_table;
	c_base_client* m_next;
	e_class_index m_class_id;
};

class c_client
{
private:
	enum e_indexes {
		_get_all_classes           = 8,
		_get_standart_recv_proxies = 48
	};

public:
	c_base_client* get_all_classes( )
	{
		using fn = c_base_client*( __thiscall* )( c_client* );
		return ( *( fn** )this )[ this->e_indexes::_get_all_classes ]( this );
	}

	c_standart_recv_proxies* get_standart_recv_proxies( )
	{
		using fn = c_standart_recv_proxies*( __thiscall* )( c_client* );
		return ( *( fn** )this )[ this->e_indexes::_get_standart_recv_proxies ]( this );
	}
};