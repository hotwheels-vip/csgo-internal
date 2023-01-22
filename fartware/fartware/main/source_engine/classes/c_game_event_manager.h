#pragma once
#include "c_key_values.h"

constexpr int EVENT_DEBUG_ID_INIT     = 42;
constexpr int EVENT_DEBUG_ID_SHUTDOWN = 13;

class c_game_event
{
public:
	virtual ~c_game_event( ) { }
	virtual const char* get_name( ) const = 0;

	virtual bool is_reliable( ) const                          = 0;
	virtual bool is_local( ) const                             = 0;
	virtual bool is_empty( const char* sz_key_name = nullptr ) = 0;

	virtual bool get_bool( const char* sz_key_name = nullptr, bool b_default = false )                         = 0;
	virtual int get_int( const char* sz_key_name = nullptr, int i_default = 0 )                                = 0;
	virtual unsigned long long get_uint64( const char* sz_key_name = nullptr, unsigned long long u_default = 0 )         = 0;
	virtual float get_float( const char* sz_key_name = nullptr, float fl_default = 0.f )                       = 0;
	virtual const char* get_string( const char* sz_key_name = nullptr, const char* sz_default = "" )           = 0;
	virtual const wchar_t* get_w_string( const char* sz_key_name = nullptr, const wchar_t* sz_default = L"" ) = 0;
	virtual const void* get_ptr( const char* sz_key_name = nullptr ) const                                     = 0;

	virtual void set_bool( const char* sz_key_name, bool b_value )                = 0;
	virtual void set_int( const char* sz_key_name, int i_value )                  = 0;
	virtual void set_uint64( const char* sz_key_name, unsigned long long u_value )     = 0;
	virtual void set_float( const char* sz_key_name, float fl_value )             = 0;
	virtual void set_string( const char* sz_key_name, const char* sz_value )      = 0;
	virtual void set_w_string( const char* sz_key_name, const wchar_t* sz_value ) = 0;
	virtual void set_ptr( const char* sz_key_name, const void* p_value )          = 0;

	virtual bool for_event_data( void* p_event ) const = 0;
};

class c_game_event_listener2
{
public:
	virtual ~c_game_event_listener2( ) { }
	virtual void fire_game_event( c_game_event* p_event ) = 0;
	virtual int get_event_debug_id( )
	{
		return m_debug_id;
	}

public:
	int m_debug_id;
};

class csvcmsg_game_event;
class c_game_event_manager2
{
public:
	virtual ~c_game_event_manager2( ) { }
	virtual int load_events_from_file( const char* sz_file_name )                                               = 0;
	virtual void reset( )                                                                                       = 0;
	virtual bool add_listener( c_game_event_listener2* p_listener, const char* sz_name, bool b_server_side )    = 0;
	virtual bool find_listener( c_game_event_listener2* p_listener, const char* sz_name )                       = 0;
	virtual void remove_listener( c_game_event_listener2* p_listener )                                          = 0;
	virtual void add_listener_global( c_game_event_listener2* p_listener, bool b_server_side )                  = 0;
	virtual c_game_event* create_new_event( const char* sz_name, bool b_force = false, int* unknown = nullptr ) = 0;
	virtual bool fire_event( c_game_event* p_event, bool b_dont_broadcast = false )                             = 0;
	virtual bool fire_event_client_side( c_game_event* p_event )                                                = 0;
	virtual c_game_event* duplicate_event( c_game_event* p_event )                                              = 0;
	virtual void free_event( c_game_event* p_event )                                                            = 0;
	virtual bool serialize_event( c_game_event* p_event, csvcmsg_game_event* p_event_msg )                      = 0;
	virtual c_game_event* unserialize_event( const csvcmsg_game_event& event_msg )                              = 0;
	virtual c_key_values* get_event_data_types( c_game_event* p_event )                                         = 0;
};