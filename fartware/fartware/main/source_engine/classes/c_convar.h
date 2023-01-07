#pragma once
#include "c_color.h"
#include "c_utl_vector.h"

// The default, no flags at all
#define FCVAR_NONE 0

// Command to ConVars and ConCommands
// ConVar Systems
#define FCVAR_UNREGISTERED    ( 1 << 0 ) // If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY ( 1 << 1 ) // Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL         ( 1 << 2 ) // defined by the game DLL
#define FCVAR_CLIENTDLL       ( 1 << 3 ) // defined by the client DLL
#define FCVAR_HIDDEN          ( 1 << 4 ) // Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED                                                                                                                              \
	( 1 << 5 ) // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY   ( 1 << 6 )  // This cvar cannot be changed by clients connected to a multiplayer server.
#define FCVAR_ARCHIVE  ( 1 << 7 )  // set to cause it to be saved to vars.rc
#define FCVAR_NOTIFY   ( 1 << 8 )  // notifies players when changed
#define FCVAR_USERINFO ( 1 << 9 )  // changes the client's info string
#define FCVAR_CHEAT    ( 1 << 14 ) // Only useable in singleplayer / debug / multiplayer & sv_cheats

#define FCVAR_PRINTABLEONLY   ( 1 << 10 ) // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED        ( 1 << 11 ) // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING ( 1 << 12 ) // never try to print that cvar

using fn_change_callback_t     = void( __cdecl* )( void*, const char*, float );
using fn_command_callback_v1_t = void( __cdecl* )( );

class c_app_system
{
private:
	virtual void function0( ) = 0;
	virtual void function1( ) = 0;
	virtual void function2( ) = 0;
	virtual void function3( ) = 0;
	virtual void function4( ) = 0;
	virtual void function5( ) = 0;
	virtual void function6( ) = 0;
	virtual void function7( ) = 0;
	virtual void function8( ) = 0;
};

using cvar_dll_identifier_t = int;

class c_conbase
{
public:
	void* m_vmt;
	c_conbase* m_next;
	bool m_registered;
	const char* m_name;
	const char* m_help_string;
	int m_flags;
	c_conbase* m_conbases;
	void* m_accessor;
};

class c_cconvar
{
private:
	enum e_indexes {
		_get_name        = 5,
		_set_value_str   = 14,
		_set_value_float = 15,
		_set_value_int   = 16,
		_set_value_color = 17
	};

public:
	const char* get_name( )
	{
		using fn = const char*( __thiscall* )( c_cconvar* );
		return ( *( fn** )this )[ this->e_indexes::_get_name ]( this );
	}

	float get_float( )
	{
		unsigned int xored_value = *reinterpret_cast< unsigned int* >( &this->m_parent->m_flvalue ) ^ reinterpret_cast< unsigned int >( this );
		return *reinterpret_cast< float* >( &xored_value );
	}

	int get_int( )
	{
		return static_cast< int >( this->m_parent->m_ivalue ^ reinterpret_cast< int >( this ) );
	}

	bool get_bool( )
	{
		return !!this->get_int( );
	}

	const char* get_string( ) const
	{
		char const* szValue = this->m_parent->m_string;
		return szValue ? szValue : "";
	}

	void set_value( const char* value )
	{
		using fn = void( __thiscall* )( c_cconvar*, const char* );
		return ( *( fn** )this )[ this->e_indexes::_set_value_str ]( this, value );
	}

	void set_value( float value )
	{
		using fn = void( __thiscall* )( c_cconvar*, float );
		return ( *( fn** )this )[ this->e_indexes::_set_value_float ]( this, value );
	}

	void set_value( int value )
	{
		using fn = void( __thiscall* )( c_cconvar*, int );
		return ( *( fn** )this )[ this->e_indexes::_set_value_int ]( this, value );
	}

	void set_value( c_color value )
	{
		using fn = void( __thiscall* )( c_cconvar*, c_color );
		return ( *( fn** )this )[ this->e_indexes::_set_value_color ]( this, value );
	}

	// void remove_callbacks( )
	//{
	//	change_callbacks.size( ) = 0;
	// }

public:
	char pad0[ 0x4 ];
	c_cconvar* m_next;
	bool m_registered;
	const char* m_name;
	const char* m_help_string;
	int m_flags;
	fn_command_callback_v1_t m_callback;
	c_cconvar* m_parent;
	const char* m_default_value;
	char* m_string;
	int m_string_length;
	float m_flvalue;
	int m_ivalue;
	bool m_has_min;
	float m_min_value;
	bool m_has_max;
	float m_max_value;
	c_utl_vector< fn_command_callback_v1_t > change_callbacks;
};

class c_concmd;
class c_color;

class c_convar : public c_app_system
{
public:
	virtual cvar_dll_identifier_t allocate_dll_identifier( )                                             = 0;
	virtual void register_con_command( c_cconvar* command_base, int default_value = 1 )                  = 0;
	virtual void unregister_con_command( c_cconvar* command_base )                                       = 0;
	virtual void unregister_con_commands( cvar_dll_identifier_t id )                                     = 0;
	virtual const char* get_command_line_value( const char* variable_name )                              = 0;
	virtual c_conbase* find_command_base( const char* name )                                             = 0;
	virtual const c_conbase* find_command_base( const char* name ) const                                 = 0;
	virtual c_cconvar* find_var( const char* variable_name )                                             = 0;
	virtual const c_cconvar* find_var( const char* variable_name ) const                                 = 0;
	virtual c_concmd* find_command( const char* name )                                                   = 0;
	virtual const c_concmd* find_command( const char* name ) const                                       = 0;
	virtual void install_global_change_callback( fn_change_callback_t callback )                         = 0;
	virtual void remove_global_change_callback( fn_change_callback_t callback )                          = 0;
	virtual void call_global_change_callbacks( c_cconvar* var, const char* old_string, float old_value ) = 0;
	virtual void install_console_display_func( void* display_func )                                      = 0;
	virtual void remove_console_display_func( void* display_func )                                       = 0;
	virtual void console_color_printf( const c_color& color, const char* format, ... ) const             = 0;
	virtual void console_printf( const char* format, ... ) const                                         = 0;
	virtual void console_d_printf( const char* format, ... ) const                                       = 0;
	virtual void revert_flagged_con_vars( int flag )                                                     = 0;
};
