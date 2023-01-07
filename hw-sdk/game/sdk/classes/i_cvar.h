#pragma once
#include "../../../utils/vfunc/vfunc.h"
#include "../../datatypes/cutl.h"
#include "../enums/cvar_ids.h"
#include "../structs/color.h"
#include "i_app_system.h"

#define CCONVAR_INTERFACE_VERSION _( "VEngineCvar007" )

struct c_var_dll_identifier;
struct con_command_base;
struct con_command;
struct i_con_var;
struct i_console_display_func;
struct i_cvar_query;

using fn_change_callback = void ( * )( i_con_var* var, const char* pOldValue, float flOldValue );

namespace sdk
{
	class con_var
	{
	public:
		void set_value( const char* value )
		{
			return virtual_func::call< void, const char* >( this, 14, value );
		}
		void set_value( float value )
		{
			return virtual_func::call< void, float >( this, 15, value );
		}
		void set_value( int value )
		{
			return virtual_func::call< void, int >( this, 16, value );
		}
		void set_value( bool value )
		{
			return virtual_func::call< void, int >( this, 16, static_cast< int >( value ) );
		}
		float get_float( )
		{
			return virtual_func::call< float >( this, 12 );
		}
		int get_int( )
		{
			return virtual_func::call< int >( this, 13 );
		}
		bool get_bool( )
		{
			return !!this->get_int( );
		}
		color get_color( )
		{
			return virtual_func::call< color >( this, 14 );
		}
		// cant have pad function here cause of build errors. RIP
	private:
		char pad_0x0000[ 0x4 ];

	public:
		con_var* next;
		std::int32_t is_registered;
		char* name;
		char* help_string;
		std::int32_t flags;

	private:
		char pad_0x0018[ 0x4 ];

	public:
		con_var* parent;
		char* default_value;
		char* string;
		std::int32_t string_length;
		float float_value;
		std::int32_t numerical_value;
		std::int32_t has_min;
		float min;
		std::int32_t has_max;
		float max;
		sdk::datatypes::c_utl_vector< unsigned int* > callbacks;
	};
	struct i_cvar : public i_app_system {
	public:
		virtual c_var_dll_identifier allocate_dll_indentifier( )                                           = 0;
		virtual void register_con_command( con_command_base* base )                                        = 0;
		virtual void unregister_con_command( con_command_base* base )                                      = 0;
		virtual void unregister_con_commands( c_var_dll_identifier id )                                    = 0;
		virtual const char* get_command_line_value( const char* name )                                     = 0;
		virtual con_command_base* find_command_base( const char* name )                                    = 0;
		virtual const con_command_base* find_command_base( const char* name ) const                        = 0;
		virtual sdk::con_var* find_var( const char* var_name )                                             = 0;
		virtual const sdk::con_var* find_var( const char* var_name ) const                                 = 0;
		virtual con_command* find_command( const char* name )                                              = 0;
		virtual const con_command* find_command( const char* name ) const                                  = 0;
		virtual void install_global_change_callback( fn_change_callback callback )                         = 0;
		virtual void remove_global_change_callback( fn_change_callback callback )                          = 0;
		virtual void call_global_change_callbacks( sdk::con_var* var, const char* old_str, float old_val ) = 0;
		virtual void install_console_display_func( i_console_display_func* func )                          = 0;
		virtual void remove_console_display_func( i_console_display_func* func )                           = 0;
		virtual void console_color_printf( const color& clr, const char* format, ... ) const               = 0;
		virtual void console_printf( const char* format, ... ) const                                       = 0;
		virtual void dconsole_dprintf( const char* format, ... ) const                                     = 0;
		virtual void rever_flagged_convars( int flag )                                                     = 0;
	};
} // namespace sdk
